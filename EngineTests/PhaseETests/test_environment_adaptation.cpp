#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/LifecycleComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "ECS/SpeciesSchema.hpp"
#include "ECS/SpeciesRegistry.hpp"
#include "ECS/SpeciesPresetFactory.hpp"

#include "Shape/Simulation/Systems/WeatherSystem.h"
#include "Shape/Simulation/Systems/BiomeSystem.h"
#include "Shape/Simulation/Systems/EnvironmentAdaptationSystem.h"

TEST_CASE("SpeciesPresetFactory: Default Primordials registration and derived stats", "[Species][Factory]") {
    Shape::SpeciesRegistry registry;
    Shape::SpeciesPresetFactory::RegisterDefaults(registry);

    REQUIRE(registry.Count() == 4);

    const auto& triangle = registry.Get(1);
    REQUIRE(triangle.geometry.vertexCount == 3);
    REQUIRE(triangle.military.combatSkillBase > 5.0f);

    const auto& quad = registry.Get(2);
    REQUIRE(quad.geometry.vertexCount == 4);
    REQUIRE(quad.biology.maxHealth == 150.0f);

    const auto& pentagon = registry.Get(3);
    REQUIRE(pentagon.geometry.vertexCount == 5);
    REQUIRE(pentagon.biology.waterRequirement < 0.5f);

    const auto& hexagon = registry.Get(4);
    REQUIRE(hexagon.geometry.vertexCount == 6);
    REQUIRE(hexagon.intelligence.rawIq == Catch::Approx(130.0f));
}

TEST_CASE("EnvironmentAdaptationSystem: Thermal stress and biome suitability evaluation", "[Environment][Adaptation]") {
    Shape::World world;
    Shape::SpeciesRegistry registry;
    Shape::SpeciesPresetFactory::RegisterDefaults(registry);

    Shape::Simulation::WeatherSystem weather_sys;
    Shape::Simulation::BiomeSystem biome_sys;
    Shape::Simulation::EnvironmentAdaptationSystem adaptation_sys(&world, &registry, &weather_sys, &biome_sys);

    Shape::Simulation::DeterministicRng rng(999);
    Shape::TickContext ctx{1, 1.0 / 60.0, 0.016, rng};

    // Spawn Nomad Pentagon entity in LushForest (pos = {0,0})
    Shape::Entity nomad = world.CreateEntity();
    Shape::TransformComponent trans;
    trans.position = {0.0f, 0.0f};

    Shape::LifecycleComponent life;
    life.health = 100.0f;
    life.energy = 100.0f;

    Shape::SpeciesIdComponent species_comp{3}; // Pentagon

    Shape::RigidBodyComponent rb;
    rb.linearDamping = 0.01f;

    world.AddComponent(nomad, trans);
    world.AddComponent(nomad, life);
    world.AddComponent(nomad, species_comp);
    world.AddComponent(nomad, rb);

    // 1. Normal spring weather update
    weather_sys.update(ctx);
    biome_sys.update(ctx);
    adaptation_sys.update(ctx);

    REQUIRE(world.HasComponent<Shape::Simulation::AdaptationStatusComponent>(nomad));
    const auto& status = world.GetComponent<Shape::Simulation::AdaptationStatusComponent>(nomad);
    REQUIRE(status.thermal_stress == Catch::Approx(0.0f)); // Normal temp (10-20C) within -25C to 50C tolerance

    // 2. Force Blizzard weather (-15C) and update
    weather_sys.force_season(Shape::Simulation::Season::Winter);
    weather_sys.force_weather(Shape::Simulation::WeatherType::Blizzard);

    Shape::TickContext winter_ctx{60, 1.0 / 60.0, 1.0, rng};
    weather_sys.update(winter_ctx);
    adaptation_sys.update(winter_ctx);

    const auto& winter_status = world.GetComponent<Shape::Simulation::AdaptationStatusComponent>(nomad);
    REQUIRE(winter_status.effective_metabolic_rate > 1.0f); // Cold weather increases metabolic drain
}
