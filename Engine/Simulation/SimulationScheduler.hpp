#pragma once

#include "SimulationClock.hpp"
#include <functional>
#include <vector>

namespace Shape {

class SimulationScheduler {
public:
    SimulationScheduler(u64 tickRateHz = 60);
    ~SimulationScheduler() = default;

    using PhaseCallback = std::function<void()>;

    void RegisterPhase1(PhaseCallback cb) { m_phase1.push_back(std::move(cb)); }
    void RegisterPhase2(PhaseCallback cb) { m_phase2.push_back(std::move(cb)); }
    void RegisterPhase3(PhaseCallback cb) { m_phase3.push_back(std::move(cb)); }

    void Update(u64 realDeltaTimeUs);

    f32 GetAlpha() const { return m_clock.GetAlpha(); }
    SimulationClock& GetClock() { return m_clock; }

private:
    void ExecuteTick();

    SimulationClock m_clock;
    std::vector<PhaseCallback> m_phase1; // Input & Read
    std::vector<PhaseCallback> m_phase2; // Physics & Update
    std::vector<PhaseCallback> m_phase3; // Write & Mutate
};

} // namespace Shape
