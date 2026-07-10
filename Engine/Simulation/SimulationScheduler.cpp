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
    // Re-create the clock at new rate, preserving tick count
    const u64 old_ticks = m_clock.GetTotalTicks();
    m_clock = SimulationClock(hz);
    m_fixed_delta = 1.0 / static_cast<double>(hz);
    (void)old_ticks; // clock resets internal tick count
}

// ─── step ─────────────────────────────────────────────────────────────────────

usize SimulationScheduler::step(double real_delta_seconds) {
    if (m_clock.IsPaused() || m_clock.GetSpeedMultiplier() <= 0.0f) return 0;

    const double scaled = real_delta_seconds * static_cast<double>(m_clock.GetSpeedMultiplier());
    m_accumulator += scaled;
    m_stats.total_real_time += real_delta_seconds;

    usize ticks_executed = 0;

    while (m_accumulator >= m_fixed_delta && ticks_executed < MAX_TICKS_PER_FRAME) {
        run_tick();
        m_accumulator -= m_fixed_delta;
        ++ticks_executed;
    }

    // Spiral-of-death prevention: if still behind, drop excess
    if (m_accumulator > m_fixed_delta) {
        m_accumulator = std::fmod(m_accumulator, m_fixed_delta);
    }

    return ticks_executed;
}

void SimulationScheduler::step_once() {
    run_tick();
}

// ─── run_tick ─────────────────────────────────────────────────────────────────

void SimulationScheduler::run_tick() {
    const auto t0 = std::chrono::high_resolution_clock::now();

    m_sim_time += m_fixed_delta;

    const TickContext ctx{
        m_clock.GetTotalTicks() + 1,  // 1-indexed tick
        m_fixed_delta,
        m_sim_time,
        m_rng
    };

    for (auto& entry : m_systems) {
        entry.system->update(ctx);
    }

    // Advance clock (also calls legacy tick callbacks if any remain)
    m_clock.Update(
        static_cast<u64>(m_fixed_delta * 1'000'000.0),
        []{} // no legacy callback — systems already ran above
    );

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
    m_stats.total_sim_time  = m_sim_time;
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

} // namespace Shape
