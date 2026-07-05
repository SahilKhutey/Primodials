#pragma once

/// @file PopulationGraph.hpp
/// @brief Live time-series tracking of population per species.

#include "Core/Platform.hpp"
#include "Math/Include/ShapeEngine/Math/Vector2.hpp"
#include "ECS/SpeciesSchema.hpp"
#include "ECS2/World2.hpp"
#include "Simulation/SimulationTime.hpp"
#include <deque>
#include <map>
#include <vector>
#include <string>

namespace Shape::Simulation::Analytics {

struct SpeciesTimePoint {
    u64 tick;
    u32 population;
};

struct SpeciesSeries {
    SpeciesId   id;
    std::string name;
    float       color_r = 1.0f;
    float       color_g = 1.0f;
    float       color_b = 1.0f;
    std::deque<SpeciesTimePoint> history;  // Most recent at back
};

class PopulationGraph {
public:
    PopulationGraph() = default;

    void set_max_history(usize points) { m_max_history = points; }
    void set_sample_interval(u64 interval) { m_sample_interval = interval; }
    
    // Call every tick. It internally checks if it's time to sample.
    void update(ECS2::World2& world, u64 current_tick);

    const std::map<SpeciesId, SpeciesSeries>& series() const { return m_series; }
    const SpeciesSeries* get_series(SpeciesId id) const;

    struct Snapshot {
        u64 tick;
        u32 total_population;
        std::vector<std::pair<SpeciesId, u32>> per_species;
    };
    Snapshot current_snapshot() const;

private:
    usize m_max_history = 600;       // 10 minutes at 1Hz sampling (if tick=60Hz)
    u64   m_sample_interval = 60;    // Sample every 60 ticks (1 sim sec)
    u64   m_last_sample = 0;
    
    std::map<SpeciesId, SpeciesSeries> m_series;
};

} // namespace Shape::Simulation::Analytics
