#pragma once

/// @file ReproductionSystem.hpp
/// @brief Handles mate-finding, gestation, and offspring spawning.
///
/// Execution order: SystemPriority::Biology (0)
/// Frequency: Every tick.
///
/// Flow:
///   1. For each creature with MatingState::None + sufficient energy + maturity:
///      query SpatialGrid for nearby same-species mates.
///   2. If suitable mate found: begin_gestation() — crossover + mutate genome,
///      set MatingState::Gestating, record gestation timer.
///   3. update_gestations(): tick timer; on completion call spawn_offspring().
///   4. spawn_offspring(): world.create() + full component set derived from
///      GenomeDecoder::apply_genome().

#include "Core/Platform.hpp"
#include "Simulation/ISimulationSystem.hpp"
#include "Simulation/Spatial/SpatialGrid.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include "ECS/SpeciesSchema.hpp"
#include "ECS2/World2.hpp"
#include <vector>

namespace Shape {

class ReproductionSystem final : public ISimulationSystem {
public:
    ReproductionSystem() = default;

    void on_register()   override;
    void on_unregister() override {}
    void update(const TickContext& ctx) override;
    int  priority() const override { return 0; }  // SystemPriority::Biology
    std::string_view name() const override { return "ReproductionSystem"; }

    void set_world(ECS2::World2* w)  noexcept { m_world = w; }
    void set_grid(SpatialGrid* g)    noexcept { m_grid = g; }

    // ── Tuneable parameters ──────────────────────────────────────────────
    float min_energy_frac    = 0.55f;  ///< Fraction of max_energy required to mate
    float mate_search_radius = 60.0f;  ///< Base spatial search radius
    float birth_energy_cost  = 30.0f;  ///< Energy deducted from mother at birth

private:
    using Rng = Simulation::DeterministicRng;

    ECS2::World2*  m_world = nullptr;
    SpatialGrid*   m_grid  = nullptr;

    // Collect candidate mates near `pos` for species `sid`.
    void find_mates(ECS2::EntityId self, Math::Vector2f pos, SpeciesId sid,
                    float radius, std::vector<ECS2::EntityId>& out) const;

    // Begin a gestation: crossover+mutate genome, tag mother.
    void begin_gestation(ECS2::EntityId mother, ECS2::EntityId father, Rng& rng);

    // Tick all ongoing gestations; spawn offspring when ready.
    void update_gestations(const TickContext& ctx, Rng& rng);

    // Spawn one offspring from mother's genome + father handle.
    ECS2::EntityId spawn_offspring(ECS2::EntityId mother, ECS2::EntityId father,
                                    const CreatureGenome& child_genome, Rng& rng);
};

} // namespace Shape
