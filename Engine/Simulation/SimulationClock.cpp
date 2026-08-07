#include "Simulation/SimulationClock.hpp"
#include "Core/Assert.hpp"

namespace Shape {

SimulationClock::SimulationClock(u64 tickRateHz)
    : m_TimeStepUs(1000000 / tickRateHz)
    , m_AccumulatorUs(0)
    , m_TotalTicks(0)
    , m_SpeedMultiplier(1.0f)
    , m_Paused(false) {
    SHAPE_ASSERT_MSG(tickRateHz > 0, "Tick rate must be greater than 0");
}

void SimulationClock::Update(u64 realDeltaTimeUs, const std::function<void()>& tickCallback) {
    if (m_Paused) return;

    // Apply speed multiplier to real delta time
    u64 scaledDeltaUs = static_cast<u64>(static_cast<f64>(realDeltaTimeUs) * m_SpeedMultiplier);

    // Limit maximum delta time per frame to prevent "spiral of death" during lag spikes
    constexpr u64 maxFrameTimeUs = 250000; // 250ms max processing per frame
    if (scaledDeltaUs > maxFrameTimeUs) {
        scaledDeltaUs = maxFrameTimeUs;
    }

    m_AccumulatorUs += scaledDeltaUs;

    // Run ticks deterministically
    while (m_AccumulatorUs >= m_TimeStepUs) {
        tickCallback();
        m_AccumulatorUs -= m_TimeStepUs;
        m_TotalTicks++;
    }
}

f32 SimulationClock::GetAlpha() const {
    if (m_Paused) return 0.0f;
    return static_cast<f32>(m_AccumulatorUs) / static_cast<f32>(m_TimeStepUs);
}

} // namespace Shape
