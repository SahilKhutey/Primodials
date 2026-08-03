#pragma once

/// @file ResourceSystem.hpp
/// @brief Food deposit management — spawning, regeneration, consumption.
///
/// Execution: priority = SystemPriority::PreSim (-50), before biology.
///
/// Each tick:
///   1. Regenerate existing food deposits (scaled by sunlight)
///   2. Respawn depleted deposits if below target count
///   3. Creatures near food consume it (energy gain via BiologyLayer.digestionEfficiency)

#include "Simulation/ISimulationSystem.hpp"
#include "Simulation/Components/ResourceComponents.hpp"
#include "Simulation/Spatial/SpatialGrid.hpp"
#include "ECS2/World2.hpp"

namespace Shape {

using DeterministicRng = Simulation::DeterministicRng;

class ResourceSystem final : public ISimulationSystem {
public:
    ResourceSystem() = default;
    ~ResourceSystem() override = default;

    void update(const TickContext& ctx) override;
    int  priority() const override { return SystemPriority::PreSim; }
    std::string_view name() const override { return "ResourceSystem"; }

    void set_world(ECS2::World2* w) { m_world = w; }
    void set_spatial_grid(SpatialGrid* g) { m_grid = g; }

    /// Spawn initial food deposits across the world.
    void seed_world(u32 count, DeterministicRng& rng);

private:
    void regenerate_food(const TickContext& ctx);
    void respawn_food(const TickContext& ctx);
    void creature_eat(const TickContext& ctx);

    Math::Vector2f random_world_pos(DeterministicRng& rng) const noexcept;

    ECS2::World2* m_world  = nullptr;
    SpatialGrid*  m_grid   = nullptr;

    // Tuning parameters
    float m_eat_radius         = 10.0f; ///< How close a creature must be to eat
    float m_energy_per_eat     = 20.0f; ///< Energy gained per eat event
    float m_respawn_probability= 0.02f; ///< Probability of spawning one new deposit per tick
};

} // namespace Shape
