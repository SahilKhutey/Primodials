#pragma once

/// @file SimulationScheduler.hpp — Production fixed-timestep tick loop.
///
/// Replaces the minimal 3-phase callback version with:
///   - Priority-ordered ISimulationSystem list
///   - TickContext (tick, fixed_delta, sim_time, rng)
///   - Speed multiplier, pause/resume
///   - Spiral-of-death cap (max 8 ticks per frame)
///   - EMA tick-time statistics
///
/// BACKWARD COMPATIBILITY:
///   Legacy RegisterPhase1/2/3 callbacks are preserved as wrappers that
///   register anonymous ISimulationSystem objects at appropriate priorities.

#include "Simulation/ISimulationSystem.hpp"
#include "Simulation/SimulationClock.hpp"
#include "Simulation/DeterministicRng.hpp"
#include "Core/Platform.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace Shape {

using DeterministicRng = Simulation::DeterministicRng;

// ─── SchedulerStats ────────────────────────────────────────────────────────

struct SchedulerStats {
    u64    total_ticks      = 0;
    double total_sim_time   = 0.0;
    double total_real_time  = 0.0;
    double avg_tick_ms      = 0.0;  ///< Exponential moving average
    double min_tick_ms      = 0.0;
    double max_tick_ms      = 0.0;
    u64    sim_rate_hz      = 0;    ///< Derived: 1000 / avg_tick_ms
    usize  last_entity_count= 0;    ///< Filled in by caller / observer
};

// ─── SimulationScheduler ────────────────────────────────────────────────────

class SimulationScheduler {
public:
    explicit SimulationScheduler(u64 tick_rate_hz = 60);
    ~SimulationScheduler() = default;

    // ── System registration ────────────────────────────────────────────────

    void register_system(ISimulationSystem& system);
    void unregister_system(ISimulationSystem& system);

    // ── Legacy compatibility ───────────────────────────────────────────────

    using PhaseCallback = std::function<void()>;
    void RegisterPhase1(PhaseCallback cb);
    void RegisterPhase2(PhaseCallback cb);
    void RegisterPhase3(PhaseCallback cb);

    /// Old-style update (microsecond delta). Bridges to new step().
    void Update(u64 real_delta_us);

    // ── New-style update ───────────────────────────────────────────────────

    /// Step the scheduler by real_delta seconds.  May fire 0–8 ticks.
    /// Returns number of ticks executed.
    usize step(double real_delta_seconds);

    /// Force exactly one tick regardless of accumulator.
    void step_once();

    // ── Time control ───────────────────────────────────────────────────────

    void  pause()         noexcept { m_clock.SetPaused(true); }
    void  resume()        noexcept { m_clock.SetPaused(false); }
    void  toggle_pause()  noexcept { m_clock.SetPaused(!m_clock.IsPaused()); }
    bool  is_paused()     const noexcept { return m_clock.IsPaused(); }

    void  set_speed(float s) noexcept { m_clock.SetSpeedMultiplier(s); }
    float speed()           const noexcept { return m_clock.GetSpeedMultiplier(); }
    void  set_tick_rate(u64 hz) noexcept;

    // ---- Catch-up / drop policy ----
    enum class DebtPolicy {
        CatchUpGradually,
        DropDebtImmediately,
        RequireUserAcknowledgment,
    };

    void set_max_ticks_per_step(size_t n) noexcept { m_max_ticks_per_step = n; }
    size_t max_ticks_per_step() const noexcept { return m_max_ticks_per_step; }

    void set_warn_ticks_per_step(size_t n) noexcept { m_warn_ticks_per_step = n; }
    size_t warn_ticks_per_step() const noexcept { return m_warn_ticks_per_step; }

    void set_max_owed_ticks(u64 n) noexcept       { m_max_owed_ticks = n; }
    u64 max_owed_ticks() const noexcept           { return m_max_owed_ticks; }

    void set_debt_policy(DebtPolicy p) noexcept { m_debt_policy = p; }
    DebtPolicy debt_policy() const noexcept     { return m_debt_policy; }

    // Observability
    u64 dropped_ticks_total() const noexcept { return m_dropped_ticks_total; }
    u64 owed_ticks_unrun() const noexcept    { return m_owed_ticks_unrun; }
    u64 last_step_ticks() const noexcept    { return m_last_step_ticks; }

    // Recovery actions
    void acknowledge_debt() noexcept;
    void reset_debt() noexcept;

    // Snapshot/restore — for save games.
    struct DebtSnapshot {
        u64 dropped_ticks_total;
        u64 owed_ticks_unrun;
        bool debt_acknowledged;
    };
    DebtSnapshot debt_snapshot() const noexcept;
    void restore_debt(const DebtSnapshot& snap) noexcept;

    // ── RNG control ────────────────────────────────────────────────────────

    void set_seed(u64 seed)              { m_rng = DeterministicRng(seed); }
    DeterministicRng& rng()              { return m_rng; }
    const DeterministicRng& rng() const  { return m_rng; }

    // ── Accessors ──────────────────────────────────────────────────────────

    SimulationClock&       clock()       noexcept { return m_clock; }
    const SimulationClock& clock() const noexcept { return m_clock; }
    float GetAlpha() const { return m_clock.GetAlpha(); }  ///< Legacy compatibility

    const SchedulerStats& stats() const noexcept { return m_stats; }
    void reset_stats() noexcept;

    u64    current_tick()  const noexcept { return m_clock.GetTotalTicks(); }
    double sim_time()      const noexcept { return static_cast<double>(m_clock.GetTotalTicks()) * m_fixed_delta; }

    static constexpr usize MAX_TICKS_PER_FRAME = 8;

private:
    void run_tick();
    void update_stats(double tick_ms);

    SimulationClock  m_clock;
    DeterministicRng m_rng{12345678ULL};
    double           m_accumulator  = 0.0;   ///< Seconds waiting for next tick
    double           m_fixed_delta  = 1.0 / 60.0;

    struct SystemEntry {
        ISimulationSystem* system;
        int priority;
    };
    std::vector<SystemEntry> m_systems;

    // Legacy anonymous systems (allocated by RegisterPhaseN wrappers)
    std::vector<std::unique_ptr<ISimulationSystem>> m_legacy_systems;

    SchedulerStats m_stats;
    double m_ema_tick_ms  = 0.0;
    double m_min_tick_ms  = 0.0;
    double m_max_tick_ms  = 0.0;

    size_t     m_max_ticks_per_step  = 32;
    size_t     m_warn_ticks_per_step = 32;
    u64        m_max_owed_ticks      = 86400;
    u64        m_dropped_ticks_total = 0;
    u64        m_owed_ticks_unrun    = 0;
    u64        m_last_step_ticks     = 0;
    bool       m_debt_acknowledged   = false;
    DebtPolicy m_debt_policy         = DebtPolicy::CatchUpGradually;
};

} // namespace Shape
