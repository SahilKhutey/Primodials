#include "Simulation/SimulationScheduler.hpp"
#include "Core/Logger.hpp"
#include "Core/Time.hpp"
#include <algorithm>
#include <chrono>

namespace Shape {

// ─── Anonymous adapter for legacy PhaseN callbacks ───────────────────────────

namespace {

struct LegacySystemAdapter final : ISimulationSystem {
    using Fn = std::function<void()>;
    Fn   m_fn;
    int  m_prio;
    const char* m_name;

    LegacySystemAdapter(Fn fn, int prio, const char* name)
        : m_fn(std::move(fn)), m_prio(prio), m_name(name) {}

    void update(const TickContext&) override { m_fn(); }
    int  priority() const override { return m_prio; }
    std::string_view name() const override { return m_name; }
};

} // anonymous namespace

// ─── Constructor ─────────────────────────────────────────────────────────────

SimulationScheduler::SimulationScheduler(u64 tick_rate_hz)
    : m_clock(tick_rate_hz)
    , m_rng(12345678ULL)
    , m_fixed_delta(1.0 / static_cast<double>(tick_rate_hz))
{}

// ─── System registration ──────────────────────────────────────────────────────

void SimulationScheduler::register_system(ISimulationSystem& system) {
    m_systems.push_back({&system, system.priority()});
    std::stable_sort(m_systems.begin(), m_systems.end(),
                     [](const SystemEntry& a, const SystemEntry& b) {
                         return a.priority < b.priority;
                     });
    system.on_register();
}

void SimulationScheduler::unregister_system(ISimulationSystem& system) {
    system.on_unregister();
    m_systems.erase(
        std::remove_if(m_systems.begin(), m_systems.end(),
                       [&](const SystemEntry& s) { return s.system == &system; }),
        m_systems.end());
}

// ─── Legacy compatibility ─────────────────────────────────────────────────────

void SimulationScheduler::RegisterPhase1(PhaseCallback cb) {
    auto adapter = std::make_unique<LegacySystemAdapter>(
        std::move(cb), SystemPriority::Spatial, "LegacyPhase1");
    register_system(*adapter);
    m_legacy_systems.push_back(std::move(adapter));
}

void SimulationScheduler::RegisterPhase2(PhaseCallback cb) {
    auto adapter = std::make_unique<LegacySystemAdapter>(
        std::move(cb), SystemPriority::Biology, "LegacyPhase2");
    register_system(*adapter);
    m_legacy_systems.push_back(std::move(adapter));
}

void SimulationScheduler::RegisterPhase3(PhaseCallback cb) {
    auto adapter = std::make_unique<LegacySystemAdapter>(
        std::move(cb), SystemPriority::Evolution, "LegacyPhase3");
    register_system(*adapter);
    m_legacy_systems.push_back(std::move(adapter));
}

void SimulationScheduler::Update(u64 real_delta_us) {
    step(static_cast<double>(real_delta_us) / 1'000'000.0);
}

// ─── set_tick_rate ────────────────────────────────────────────────────────────

void SimulationScheduler::set_tick_rate(u64 hz) noexcept {
    const u64 old_ticks = m_clock.GetTotalTicks();
    m_clock = SimulationClock(hz);
    m_clock.SetTotalTicks(old_ticks);
    m_fixed_delta = 1.0 / static_cast<double>(hz);
}

// ─── step ─────────────────────────────────────────────────────────────────────

usize SimulationScheduler::step(double real_delta_seconds) {
    SHAPE_ASSERT_MSG(std::isfinite(real_delta_seconds), "step: real_delta is not finite");
    SHAPE_ASSERT_MSG(real_delta_seconds >= 0.0,        "step: real_delta is negative");
    SHAPE_ASSERT_MSG(real_delta_seconds < 86400.0,    "step: real_delta exceeds 24h — likely a bug");

    if (m_clock.IsPaused() || m_clock.GetSpeedMultiplier() <= 0.0f) {
        return 0;
    }

    m_stats.total_real_time += real_delta_seconds;

    const double scaled_delta = real_delta_seconds * static_cast<double>(m_clock.GetSpeedMultiplier());
    m_accumulator += scaled_delta;

    u64 owed_total = static_cast<u64>(m_accumulator / m_fixed_delta);
    const u64 budget = static_cast<u64>(m_max_ticks_per_step);

    // Hard ceiling
    if (owed_total > m_max_owed_ticks) {
        if (m_debt_policy == DebtPolicy::RequireUserAcknowledgment) {
            m_last_step_ticks = 0;
            m_owed_ticks_unrun = owed_total;
            return 0; // Frozen until user acks
        }
        if (m_debt_policy == DebtPolicy::DropDebtImmediately) {
            const u64 dropped = owed_total - 1;
            m_dropped_ticks_total += dropped;
            m_accumulator = m_fixed_delta; // Keep one tick's worth
            owed_total = 1;
        } else {
            // CatchUpGradually
            SHAPE_LOG_WARN("Catch-up debt exceeds hard ceiling: {} ticks owed, max={}. "
                "Continuing at per-step budget of {} ticks/frame. "
                "This will take ~{} frames to clear at the current rate.",
                owed_total, m_max_owed_ticks, budget,
                (owed_total + budget - 1) / budget);
        }
    }

    u64 ticks_to_run = owed_total;
    if (owed_total > budget) {
        if (m_debt_policy == DebtPolicy::DropDebtImmediately) {
            const u64 dropped = owed_total - 1;
            m_dropped_ticks_total += dropped;
            m_accumulator = m_fixed_delta; // Keep one tick's worth
            ticks_to_run = 1;
            m_owed_ticks_unrun = 0;
        } else {
            ticks_to_run = budget;
            m_owed_ticks_unrun = owed_total - budget;
        }
    } else {
        m_owed_ticks_unrun = 0;
    }

    for (u64 i = 0; i < ticks_to_run; ++i) {
        run_tick();
        m_accumulator -= m_fixed_delta;
    }

    m_last_step_ticks = ticks_to_run;

    // Log warning if budget was exhausted (and not dropping immediately)
    if (ticks_to_run == budget && m_owed_ticks_unrun > 0 && m_debt_policy == DebtPolicy::CatchUpGradually) {
        SHAPE_LOG_WARN("Catch-up budget exhausted: ran {} of {} owed ticks this step. "
            "{} ticks remaining for next step. Total dropped: {}.",
            ticks_to_run, owed_total, m_owed_ticks_unrun,
            m_dropped_ticks_total);
    }

    // Defensive check
    if (m_accumulator > 3600.0) {
        SHAPE_LOG_ERROR("Accumulator reached {}s without hitting max_owed_ticks cap. "
            "This is a bug. Resetting to 0. sim_time may be inaccurate.",
            m_accumulator);
        m_accumulator = 0.0;
        m_owed_ticks_unrun = 0;
    }

    m_clock.SetAccumulatorUs(static_cast<u64>(m_accumulator * 1'000'000.0));

    return ticks_to_run;
}

void SimulationScheduler::step_once() {
    run_tick();
}

// ─── run_tick ─────────────────────────────────────────────────────────────────

void SimulationScheduler::run_tick() {
    const auto t0 = std::chrono::high_resolution_clock::now();

    // Advance clock tick count directly (bypassing speed double-scaling)
    m_clock.IncrementTicks();

    const TickContext ctx{
        m_clock.GetTotalTicks(),  // 1-indexed tick
        m_fixed_delta,
        static_cast<double>(m_clock.GetTotalTicks()) * m_fixed_delta,
        m_rng
    };

    for (auto& entry : m_systems) {
        entry.system->update(ctx);
    }

    const auto t1 = std::chrono::high_resolution_clock::now();
    const double tick_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    update_stats(tick_ms);
}

// ─── Stats ────────────────────────────────────────────────────────────────────

void SimulationScheduler::update_stats(double tick_ms) {
    constexpr double EMA_ALPHA = 0.02;
    m_ema_tick_ms = m_ema_tick_ms * (1.0 - EMA_ALPHA) + tick_ms * EMA_ALPHA;
    if (m_min_tick_ms == 0.0) m_min_tick_ms = tick_ms;
    else m_min_tick_ms = std::min(m_min_tick_ms, tick_ms);
    m_max_tick_ms = std::max(m_max_tick_ms, tick_ms);

    m_stats.total_ticks     = m_clock.GetTotalTicks();
    m_stats.total_sim_time  = sim_time();
    m_stats.avg_tick_ms     = m_ema_tick_ms;
    m_stats.min_tick_ms     = m_min_tick_ms;
    m_stats.max_tick_ms     = m_max_tick_ms;
    m_stats.sim_rate_hz     = m_ema_tick_ms > 0.0
                              ? static_cast<u64>(1000.0 / m_ema_tick_ms)
                              : 0;
}

void SimulationScheduler::reset_stats() noexcept {
    m_stats       = {};
    m_ema_tick_ms = 0.0;
    m_min_tick_ms = 0.0;
    m_max_tick_ms = 0.0;
}

void SimulationScheduler::acknowledge_debt() noexcept {
    m_debt_acknowledged = true;
    m_owed_ticks_unrun = 0;
    m_accumulator = 0.0;
    m_clock.SetAccumulatorUs(0);
    SHAPE_LOG_INFO("Debt acknowledged and cleared by user.");
}

void SimulationScheduler::reset_debt() noexcept {
    m_owed_ticks_unrun = 0;
    m_dropped_ticks_total = 0;
    m_accumulator = 0.0;
    m_clock.SetAccumulatorUs(0);
    m_debt_acknowledged = false;
    SHAPE_LOG_INFO("Debt reset.");
}

SimulationScheduler::DebtSnapshot SimulationScheduler::debt_snapshot() const noexcept {
    return {
        m_dropped_ticks_total,
        m_owed_ticks_unrun,
        m_debt_acknowledged
    };
}

void SimulationScheduler::restore_debt(const DebtSnapshot& snap) noexcept {
    m_dropped_ticks_total = snap.dropped_ticks_total;
    m_owed_ticks_unrun    = snap.owed_ticks_unrun;
    m_debt_acknowledged   = snap.debt_acknowledged;
}

} // namespace Shape
