#include "Simulation/Genetics/GeneticsSystem.hpp"
#include "Core/Logger.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>

namespace Shape {

// ─── Static utility implementations ──────────────────────────────────────────

CreatureGenome GeneticsSystem::crossover(const CreatureGenome& a,
                                          const CreatureGenome& b,
                                          DeterministicRng& rng) noexcept {
    CreatureGenome child;
    for (u8 i = 0; i < CreatureGenome::GENE_COUNT; ++i) {
        // Uniform crossover: each gene independently from either parent
        child.genes[i] = (rng.NextF32() < 0.5f) ? a.genes[i] : b.genes[i];
    }
    child.parent_a_hash = static_cast<u32>(a.lineage_hash);
    child.parent_b_hash = static_cast<u32>(b.lineage_hash);
    child.lineage_hash  = compute_hash(child);
    return child;
}

void GeneticsSystem::mutate(CreatureGenome& genome,
                             float mutation_rate,
                             float mutation_amplitude,
                             DeterministicRng& rng) noexcept {
    for (u8 i = 0; i < CreatureGenome::GENE_COUNT; ++i) {
        if (rng.NextF32() < mutation_rate) {
            // Box–Muller-free Gaussian approximation: sum of 3 uniform ≈ Normal
            const float n = (rng.NextF32() + rng.NextF32() + rng.NextF32() - 1.5f)
                          * (2.0f / 1.5f); // scale to ~[-1,1]
            genome.genes[i] = std::clamp(
                genome.genes[i] + n * mutation_amplitude, 0.0f, 1.0f);
        }
    }
    genome.lineage_hash = compute_hash(genome);
}

u64 GeneticsSystem::compute_hash(const CreatureGenome& genome) noexcept {
    // FNV-1a 64-bit over raw gene bytes
    constexpr u64 FNV_OFFSET = 14695981039346656037ULL;
    constexpr u64 FNV_PRIME  =        1099511628211ULL;
    u64 h = FNV_OFFSET;
    const u8* bytes = reinterpret_cast<const u8*>(genome.genes.data());
    for (usize i = 0; i < sizeof(float) * CreatureGenome::GENE_COUNT; ++i) {
        h ^= static_cast<u64>(bytes[i]);
        h *= FNV_PRIME;
    }
    return h;
}

float GeneticsSystem::genomic_distance(const CreatureGenome& a,
                                        const CreatureGenome& b) noexcept {
    float sum_sq = 0.0f;
    for (u8 i = 0; i < CreatureGenome::GENE_COUNT; ++i) {
        const float d = a.genes[i] - b.genes[i];
        sum_sq += d * d;
    }
    // Normalised Euclidean distance (max possible = sqrt(GENE_COUNT))
    const float max_dist = std::sqrt(static_cast<float>(CreatureGenome::GENE_COUNT));
    return std::sqrt(sum_sq) / max_dist;
}

CreatureGenome GeneticsSystem::from_species(const Species& species) noexcept {
    CreatureGenome g;
    // Map first 16 species genome genes directly
    const usize n = std::min(static_cast<usize>(CreatureGenome::GENE_COUNT),
                              static_cast<usize>(species.genome.count));
    for (usize i = 0; i < n; ++i) {
        // Normalize gene value to [0,1] (gene.value can be any float in schema)
        g.genes[i] = std::clamp(species.genome.genes[i].value, 0.0f, 1.0f);
    }
    // Encode core attributes into remaining genes
    if constexpr (CreatureGenome::GENE_COUNT >= 16) {
        g.genes[0]  = std::clamp(species.biology.metabolismRate / 5.0f, 0.0f, 1.0f);
        g.genes[1]  = std::clamp(species.biology.lifespan / 72000.0f, 0.0f, 1.0f);
        g.genes[2]  = std::clamp(species.physics.maxSpeed / 200.0f, 0.0f, 1.0f);
        g.genes[3]  = std::clamp(species.psychology.aggression, 0.0f, 1.0f);
        g.genes[4]  = std::clamp(species.intelligence.rawIq / 200.0f, 0.0f, 1.0f);
        g.genes[5]  = std::clamp(species.reproduction.fertilityBaseline, 0.0f, 1.0f);
        g.genes[6]  = std::clamp(species.genetics.mutationRate * 20.0f, 0.0f, 1.0f);
        g.genes[7]  = std::clamp(species.biology.digestionEfficiency, 0.0f, 1.0f);
        g.genes[8]  = std::clamp(species.evolution.adaptiveRadius, 0.0f, 1.0f);
        g.genes[9]  = std::clamp(species.society.socialNeed, 0.0f, 1.0f);
        g.genes[10] = std::clamp(species.biology.regenerationRate, 0.0f, 1.0f);
        g.genes[11] = std::clamp(species.psychology.curiosity, 0.0f, 1.0f);
        g.genes[12] = std::clamp(species.chemistry.pheromoneStrength, 0.0f, 1.0f);
        g.genes[13] = std::clamp(species.evolution.speciationThreshold, 0.0f, 1.0f);
        g.genes[14] = std::clamp(species.resources.territoryNeed / 500.0f, 0.0f, 1.0f);
        g.genes[15] = std::clamp(species.civilization.cohesionStrength, 0.0f, 1.0f);
    }
    g.lineage_hash = compute_hash(g);
    return g;
}

// ─── update ───────────────────────────────────────────────────────────────────

void GeneticsSystem::update(const TickContext& ctx) {
    if (!m_world) return;
    process_reproduction(ctx);
}

void GeneticsSystem::process_reproduction(const TickContext& ctx) {
    // Iterate creatures ready to reproduce
    // For now this is a placeholder — the full implementation requires
    // ECS2 World2 query over ReproductionState + CreatureGenome
    // which will be wired when the demo is set up.
    //
    // The static utility functions (crossover, mutate, etc.) are
    // fully implemented and tested independently.
    (void)ctx;
}

} // namespace Shape
