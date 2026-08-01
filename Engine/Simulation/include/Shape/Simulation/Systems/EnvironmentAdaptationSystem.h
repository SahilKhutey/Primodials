// Shape/Simulation/Systems/EnvironmentAdaptationSystem.h
#pragma once

#include "Simulation/ISimulationSystem.hpp"
#include "ECS/World.hpp"
#include "ECS/SpeciesSchema.hpp"
#include "ECS/SpeciesRegistry.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/LifecycleComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "ECS/ComponentTypes.hpp"
#include "Shape/Simulation/Systems/WeatherSystem.h"
#include "Shape/Simulation/Systems/BiomeSystem.h"
#include <string_view>

namespace Shape::Simulation {

/// Environmental adaptation and stress status per entity.
struct AdaptationStatusComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::AdaptationStatusComponent);

    float thermal_stress = 0.0f;       // 0.0 = comfortable, >0.0 = suffering heat/cold stress
    float moisture_satisfaction = 1.0f;// 0.0 = severe dehydration, 1.0 = hydrated
    float effective_metabolic_rate = 1.0f; // Scaled metabolism multiplier
    float movement_speed_multiplier = 1.0f;// Biome-adapted movement speed
    float camouflage_factor = 0.0f;    // 0.0 = fully exposed, 1.0 = perfectly camouflaged
    bool is_in_preferred_biome = false;
};

/// Evaluates species attribute compatibility against environmental simulation states.
class EnvironmentAdaptationSystem : public ISimulationSystem {
public:
    EnvironmentAdaptationSystem() = default;
    EnvironmentAdaptationSystem(World* world, SpeciesRegistry* registry, WeatherSystem* weather, BiomeSystem* biome);
    virtual ~EnvironmentAdaptationSystem() = default;

    void set_context(World* world, SpeciesRegistry* registry, WeatherSystem* weather, BiomeSystem* biome) noexcept {
        m_world = world;
        m_registry = registry;
        m_weather_system = weather;
        m_biome_system = biome;
    }

    void update(const TickContext& ctx) override;
    std::string_view name() const override { return "EnvironmentAdaptationSystem"; }
    int priority() const override { return -30; } // Post-Weather & Biome setup, pre-AI & physics

    /// Calculate biome suitability score (0.0 to 1.0) for a given species in a biome.
    static float calculate_biome_suitability(const Species& species, BiomeType biome);

private:
    World* m_world = nullptr;
    SpeciesRegistry* m_registry = nullptr;
    WeatherSystem* m_weather_system = nullptr;
    BiomeSystem* m_biome_system = nullptr;
};

} // namespace Shape::Simulation
