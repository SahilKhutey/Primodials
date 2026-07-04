#pragma once

#include "Core/Platform.hpp"
#include <functional>

namespace Shape {

class SimulationClock {
public:
    SimulationClock(u64 tickRateHz = 60);
    ~SimulationClock() = default;

    // Advances the clock by real-world delta time in microseconds
    // Calls the tickCallback for each simulation tick that needs to occur
    void Update(u64 realDeltaTimeUs, const std::function<void()>& tickCallback);

    // Speed Controls
    void SetSpeedMultiplier(f32 multiplier) { m_SpeedMultiplier = multiplier; }
    f32 GetSpeedMultiplier() const { return m_SpeedMultiplier; }

    void SetPaused(bool paused) { m_Paused = paused; }
    bool IsPaused() const { return m_Paused; }

    // Time Accessors
    u64 GetTotalTicks() const { return m_TotalTicks; }
    u64 GetTimeStepUs() const { return m_TimeStepUs; }
    f32 GetTimeStepSeconds() const { return static_cast<f32>(m_TimeStepUs) / 1000000.0f; }

    // Decoupled interpolation factor (for rendering smooth transitions between ticks)
    f32 GetAlpha() const;

private:
    u64 m_TimeStepUs;       // Fixed length of a single simulation tick in microseconds
    u64 m_AccumulatorUs;    // Time waiting to be processed in microseconds
    u64 m_TotalTicks;       // Total simulation ticks processed since start
    f32 m_SpeedMultiplier;  // Speed scale factor (1.0 = normal, 2.0 = double, etc.)
    bool m_Paused;
};

} // namespace Shape
