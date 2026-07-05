#pragma once

/// @file SimulationMetrics.hpp
/// @brief Aggregated simulation metrics over time.

#include "Core/Platform.hpp"
#include <vector>

namespace Shape::Simulation::Analytics {

struct FrameMetrics {
    u64    tick;
    u32    population;
    u32    food_count;
    u32    mating_count;
    u32    combat_count;
    double avg_energy;
    double avg_age;
};

class SimulationMetrics {
public:
    void record(FrameMetrics m) {
        m_history.push_back(std::move(m));
        while (m_history.size() > 1000) m_history.erase(m_history.begin());
    }

    const std::vector<FrameMetrics>& history() const { return m_history; }
    FrameMetrics latest() const { return m_history.empty() ? FrameMetrics{} : m_history.back(); }

private:
    std::vector<FrameMetrics> m_history;
};

} // namespace Shape::Simulation::Analytics
