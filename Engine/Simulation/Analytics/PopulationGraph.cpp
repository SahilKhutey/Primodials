#include "Simulation/Analytics/PopulationGraph.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
// TODO: inject SpeciesRegistry once available in Phase E
// #include "ECS/SpeciesRegistry.hpp"

namespace Shape::Simulation::Analytics {

void PopulationGraph::update(ECS2::World2& world, u64 current_tick) {
    if (current_tick - m_last_sample < m_sample_interval) return;
    m_last_sample = current_tick;

    // Count population per species
    std::map<SpeciesId, u32> pop;
    world.for_each<CreatureState>([&](const CreatureState& cs) {
        if (cs.is_alive) {
            pop[cs.species_id]++;
        }
    });

    // Update time-series
    for (const auto& [sid, count] : pop) {
        SpeciesSeries& series = m_series[sid];
        series.id = sid;
        
        // Mock names and colors until SpeciesRegistry is fully injected in Phase E
        if (series.name.empty()) {
            series.name = "Species " + std::to_string(sid);
            // Default color generator based on ID
            series.color_r = (sid * 37 % 100) / 100.0f;
            series.color_g = (sid * 71 % 100) / 100.0f;
            series.color_b = (sid * 97 % 100) / 100.0f;
        }

        series.history.push_back({current_tick, count});
        while (series.history.size() > m_max_history) {
            series.history.pop_front();
        }
    }

    // Remove extinct species from the graph tracking if their history fully ages out,
    // or just leave them at 0. For Phase D we keep them at 0 by appending 0 if they
    // aren't in the pop map.
    for (auto& [sid, series] : m_series) {
        if (pop.find(sid) == pop.end()) {
            series.history.push_back({current_tick, 0});
            while (series.history.size() > m_max_history) {
                series.history.pop_front();
            }
        }
    }
}

const SpeciesSeries* PopulationGraph::get_series(SpeciesId id) const {
    auto it = m_series.find(id);
    return it != m_series.end() ? &it->second : nullptr;
}

PopulationGraph::Snapshot PopulationGraph::current_snapshot() const {
    Snapshot s;
    s.tick = m_last_sample;
    s.total_population = 0;
    for (const auto& [sid, series] : m_series) {
        if (!series.history.empty()) {
            const u32 pop = series.history.back().population;
            if (pop > 0) {
                s.per_species.push_back({sid, pop});
                s.total_population += pop;
            }
        }
    }
    return s;
}

} // namespace Shape::Simulation::Analytics
