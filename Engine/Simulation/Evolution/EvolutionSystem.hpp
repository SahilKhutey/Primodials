#pragma once

/// @file EvolutionSystem.hpp
/// @brief Natural selection, fitness scoring, speciation, and extinction.
///
/// Runs every SELECTION_INTERVAL ticks (every ~10 sim-seconds at 60 Hz).
/// 
/// Per selection cycle:
///   1. Compute fitness for all living creatures
///   2. If population > carrying_capacity: destroy lowest-fitness entities
///   3. Detect speciation events (genomic distance > threshold → new species)
///   4. Detect extinction (species < MIN_VIABLE_POPULATION)
///   5. Update SimulationStats in WorldState resource
///
/// DETERMINISM: all random choices use TickContext::rng only.

#include "Simulation/ISimulationSystem.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include "ECS/SpeciesSchema.hpp"
#include "ECS2/World2.hpp"

namespace Shape {

class EvolutionSystem final : public ISimulationSystem {
public:
    EvolutionSystem() = default;
    ~EvolutionSystem() override = default;

    void update(const TickContext& ctx) override;
    int  priority() const override { return SystemPriority::Evolution; }
    std::string_view name() const override { return "EvolutionSystem"; }

    void set_world(ECS2::World2* w) { m_world = w; }

    // ── Fitness function ──────────────────────────────────────────────────

    /// Compute fitness score for a creature.
    /// Returns value in [0, 1] — higher = more fit.
    static float compute_fitness(const CreatureState& state,
                                 const ReproductionState& repro,
                                 const Species& species) noexcept;

    /// Genomic speciation threshold from species EvolutionLayer.
    static float speciation_threshold(const Species& s) noexcept {
        return s.evolution.speciationThreshold;
    }

    // ── Tuning ────────────────────────────────────────────────────────────

    void set_selection_interval(u64 ticks) { m_selection_interval = ticks; }
    void set_min_viable_pop(u32 n)         { m_min_viable_pop = n; }

    static constexpr u32 DEFAULT_SELECTION_INTERVAL = 600; ///< 10 seconds at 60 Hz
    static constexpr u32 MIN_VIABLE_POPULATION      = 3;

private:
    void run_selection(const TickContext& ctx);

    ECS2::World2* m_world              = nullptr;
    u64           m_selection_interval = DEFAULT_SELECTION_INTERVAL;
    u32           m_min_viable_pop     = MIN_VIABLE_POPULATION;
};

} // namespace Shape
