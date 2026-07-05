#include "Simulation/Evolution/SpeciationSystem.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include "Simulation/Genome/GenomeDecoder.hpp"
#include "Simulation/SimulationState.hpp"
#include "Core/Logger.hpp"
#include <map>
#include <vector>

namespace Shape {

void SpeciationSystem::on_register() {
    SHAPE_LOG_INFO("Evolution", "SpeciationSystem registered");
}

void SpeciationSystem::update(const TickContext& ctx) {
    if (!m_world) return;
    if (ctx.tick - m_last_check < check_interval_ticks) return;
    m_last_check = ctx.tick;

    // We scan genomes per species to find divergence.
    // To ensure determinism, we use std::map (sorted by species ID, then hash).
    std::map<SpeciesId, std::map<u32, u32>> genome_counts; // hash -> count

    m_world->for_each<CreatureState, CreatureGenome>(
        [&](const CreatureState& cs, const CreatureGenome& g) {
        if (!cs.is_alive) return;
        
        // Compute a simple hash of the genome floats (deterministic enough for this)
        u32 hash = 2166136261u;
        for (u8 i = 0; i < CreatureGenome::GENE_COUNT; ++i) {
            // Reinterpret float as bits for stable hashing across platforms
            u32 bits = 0;
            std::memcpy(&bits, &g.genes[i], sizeof(bits));
            hash ^= bits;
            hash *= 16777619u;
        }
        
        genome_counts[cs.species_id][hash]++;
    });

    // We don't auto-create species yet (Phase E), but we track divergence metric
    u32 speciation_events = 0;
    
    for (const auto& [sid, counts] : genome_counts) {
        if (counts.empty()) continue;
        
        // Find most common genome hash
        u32 dominant_hash = 0;
        u32 max_count = 0;
        for (const auto& [hash, count] : counts) {
            if (count > max_count) {
                max_count = count;
                dominant_hash = hash;
            }
        }
        
        // We log the diversity count
        if (counts.size() > 20) {
            SHAPE_LOG_DEBUG("Evolution", "Species {} shows high diversity ({} unique genomes)", 
                            sid, counts.size());
            speciation_events++;
        }
    }

    if (speciation_events > 0) {
        auto& ws = m_world->resource<WorldState>();
        // Increment global stats (Phase D is observational)
        ws.stats.total_speciations += speciation_events;
    }
}

} // namespace Shape
