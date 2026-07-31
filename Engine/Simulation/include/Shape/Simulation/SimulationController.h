// Shape/Simulation/SimulationController.h
#pragma once

#include "Simulation/SimulationClock.hpp"
#include "Simulation/SimulationScheduler.hpp"
#include <string_view>

namespace Shape::Simulation {

enum class SimulationState {
    Stopped,
    Running,
    Paused,
    StepSingleTick,
    FastForward
};

class SimulationController {
public:
    SimulationController();
    explicit SimulationController(SimulationClock* clock, SimulationScheduler* scheduler);
    ~SimulationController() = default;

    void set_clock(SimulationClock* clock) noexcept { m_clock = clock; }
    void set_scheduler(SimulationScheduler* scheduler) noexcept { m_scheduler = scheduler; }

    // ── Controls ──────────────────────────────────────────────────────────
    void Play();
    void Pause();
    void StepSingleTick();
    void FastForward(float time_scale);
    void Stop();

    void SetTimeScale(float scale);
    float GetTimeScale() const noexcept;

    void SetSpeedPreset(uint32_t preset); // 1 = 1x, 2 = 2x, 3 = 5x, 4 = 10x

    // ── State Accessors ───────────────────────────────────────────────────
    SimulationState GetState() const noexcept { return m_state; }
    bool IsRunning() const noexcept { return m_state == SimulationState::Running || m_state == SimulationState::FastForward; }
    bool IsPaused() const noexcept { return m_state == SimulationState::Paused; }
    uint64_t GetCurrentTick() const noexcept;

    // Advance clock by real delta time (microseconds)
    void Update(uint64_t real_delta_us);

private:
    SimulationClock* m_clock = nullptr;
    SimulationScheduler* m_scheduler = nullptr;
    SimulationState m_state = SimulationState::Stopped;
    float m_saved_time_scale = 1.0f;
};

} // namespace Shape::Simulation
