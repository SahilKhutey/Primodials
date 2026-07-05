#pragma once

/// @file GeneticsSystem.hpp
/// @brief Genome crossover and mutation applied at reproduction events.
///
/// Listens for entities with ReproductionState::ready == true.
/// Performs genome crossover from parent data, then per-gene mutation.
/// Posts offspring creation to CommandBuffer2 after genetics are resolved.
///
/// DETERMINISM: uses TickContext::rng exclusively.

#include "Simulation/ISimulationSystem.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include "ECS/SpeciesSchema.hpp"
#include "ECS2/World2.hpp"

namespace Shape {

using DeterministicRng = Simulation::DeterministicRng;

class GeneticsSystem final : public ISimulationSystem {
public:
    GeneticsSystem() = default;
    ~GeneticsSystem() override = default;

    void update(const TickContext& ctx) override;
    int  priority() const override { return SystemPriority::Genetics; }
    std::string_view name() const override { return "GeneticsSystem"; }

    void set_world(ECS2::World2* w) { m_world = w; }

    // ── Static genetics utilities (also used by EvolutionSystem) ──────────

    /// Crossover: child gene[i] = (rng < 0.5) ? a[i] : b[i]  (uniform crossover)
    static CreatureGenome crossover(const CreatureGenome& a,
                                    const CreatureGenome& b,
                                    DeterministicRng& rng) noexcept;

    /// Mutation: perturb each gene by Gaussian noise scaled by mutation_rate.
    static void mutate(CreatureGenome& genome,
                       float mutation_rate,
                       float mutation_amplitude,
                       DeterministicRng& rng) noexcept;

    /// Compute lineage hash from gene values (for speciation distance checks).
    static u64 compute_hash(const CreatureGenome& genome) noexcept;

    /// Genomic distance between two genomes [0,1].
    static float genomic_distance(const CreatureGenome& a,
                                  const CreatureGenome& b) noexcept;

    /// Build a CreatureGenome from a Species template genome
    /// (maps 64-gene SpeciesGenome → 16-gene CreatureGenome).
    static CreatureGenome from_species(const Species& species) noexcept;

private:
    void process_reproduction(const TickContext& ctx);

    ECS2::World2* m_world = nullptr;

    // Tuning
    float m_mutation_amplitude = 0.05f; ///< Max per-gene perturbation
};

} // namespace Shape
