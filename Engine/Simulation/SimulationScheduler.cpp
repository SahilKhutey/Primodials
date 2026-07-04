#include "SimulationScheduler.hpp"

namespace Shape {

SimulationScheduler::SimulationScheduler(u64 tickRateHz) 
    : m_clock(tickRateHz) 
{
}

void SimulationScheduler::Update(u64 realDeltaTimeUs) {
    m_clock.Update(realDeltaTimeUs, [this]() {
        ExecuteTick();
    });
}

void SimulationScheduler::ExecuteTick() {
    // Phase 1: Input & Read
    for (auto& cb : m_phase1) {
        cb();
    }

    // Phase 2: Physics & Update
    for (auto& cb : m_phase2) {
        cb();
    }

    // Phase 3: Write & Mutate
    for (auto& cb : m_phase3) {
        cb();
    }
}

} // namespace Shape
