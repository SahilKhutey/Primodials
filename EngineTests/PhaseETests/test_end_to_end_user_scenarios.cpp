#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

// Simulation Systems & Components
#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "ECS/MindComponent.hpp"
#include "ECS/LifecycleComponent.hpp"
#include "ECS/StorageComponent.hpp"
#include "ECS/FlockComponent.hpp"
#include "ECS/CivilizationComponent.hpp"

#include "Shape/Simulation/Systems/WeatherSystem.h"
#include "Shape/Simulation/Systems/BiomeSystem.h"
#include "Shape/Simulation/Systems/BuildingSystem.h"
#include "Shape/Simulation/Systems/TechnologySystem.h"
#include "Shape/Simulation/Systems/DiplomacySystem.h"
#include "Shape/Simulation/Systems/GroupBehaviorSystem.h"
#include "Shape/Simulation/Systems/QLearningSystem.h"
#include "Civilization/CivilizationSystem.hpp"

#include "Shape/Simulation/SimulationController.h"
#include "Simulation/SimulationClock.hpp"
#include "Simulation/SimulationScheduler.hpp"
#include "Rendering/CameraController.h"
#include "Rendering/GameInputController.h"

TEST_CASE("User Scenario 1: ALife Ecosystem & Weather Survival", "[Scenario][ALife]") {
    Shape::World world;
    Shape::Simulation::WeatherSystem weather_sys;
    Shape::Simulation::BiomeSystem biome_sys;
    Shape::Simulation::GroupBehaviorSystem flock_sys(&world);
    Shape::Simulation::QLearningSystem q_sys(&world);

    Shape::Simulation::DeterministicRng rng(777);
    Shape::TickContext ctx{1, 1.0 / 60.0, 0.016, rng};

    // 1. Spawn a flock of 5 entities in LushForest
    std::vector<Shape::Entity> flock_entities;
    for (int i = 0; i < 5; ++i) {
        Shape::Entity e = world.CreateEntity();
        Shape::TransformComponent t;
        t.position = {static_cast<float>(i * 4), 0.0f};

        Shape::RigidBodyComponent rb;
        rb.velocity = {5.0f, 0.0f};

        Shape::FlockComponent f;
        f.flock_id = 1;
        f.perception_radius = 50.0f;

        Shape::MindComponent mind;
        Shape::LifecycleComponent life;
        life.health = 100.0f;
        life.energy = 80.0f;

        world.AddComponent(e, t);
        world.AddComponent(e, rb);
        world.AddComponent(e, f);
        world.AddComponent(e, mind);
        world.AddComponent(e, life);

        flock_entities.push_back(e);
    }

    // 2. Simulate 60 ticks under normal weather
    for (int tick = 0; tick < 60; ++tick) {
        weather_sys.update(ctx);
        biome_sys.update(ctx);
        flock_sys.update(ctx);
        q_sys.update(ctx);
    }

    // Verify flocking cohesion and separation forces were applied
    const auto& rb0 = world.GetComponent<Shape::RigidBodyComponent>(flock_entities[0]);
    REQUIRE(rb0.force.LengthSquared() > 0.0f);

    // Verify spatial biome property query for entities
    const auto& t0 = world.GetComponent<Shape::TransformComponent>(flock_entities[0]);
    auto biome_props = biome_sys.get_properties_at(t0.position);
    REQUIRE(biome_props.fertility_multiplier > 0.0f);

    // 3. Trigger extreme weather event (Heatwave)
    weather_sys.force_season(Shape::Simulation::Season::Summer);
    weather_sys.force_weather(Shape::Simulation::WeatherType::Heatwave);

    Shape::TickContext summer_ctx{61, 1.0 / 60.0, 1.0, rng};
    weather_sys.update(summer_ctx);

    const auto& env_state = weather_sys.state();
    REQUIRE(env_state.current_weather == Shape::Simulation::WeatherType::Heatwave);
    REQUIRE(env_state.temperature_celsius >= 30.0f);
}

TEST_CASE("User Scenario 2: Colony Construction & Tech Research", "[Scenario][Civilization]") {
    Shape::World world;
    Shape::Simulation::BuildingSystem building_sys(&world);
    Shape::CivilizationSystem civ_sys;

    Shape::Simulation::DeterministicRng rng(888);
    Shape::TickContext ctx{1, 1.0 / 60.0, 0.016, rng};

    // 1. Create Colony Depot and Farm
    Shape::Entity depot = world.CreateEntity();
    Shape::Simulation::BuildingComponent depot_comp;
    depot_comp.building_type = Shape::Simulation::BuildingType::Depot;
    depot_comp.faction_id = 1;
    depot_comp.construction_progress = 1.0f; // Fully built
    world.AddComponent(depot, depot_comp);

    Shape::Entity farm = world.CreateEntity();
    Shape::Simulation::BuildingComponent farm_comp;
    farm_comp.building_type = Shape::Simulation::BuildingType::Farm;
    farm_comp.faction_id = 1;
    farm_comp.construction_progress = 0.5f; // Half built
    world.AddComponent(farm, farm_comp);

    // Storage for resource accumulation & tech research
    Shape::Entity civ_entity = world.CreateEntity();
    world.AddComponent(civ_entity, Shape::TransformComponent{});
    world.AddComponent(civ_entity, Shape::CivilizationComponent{Shape::FactionAlignment::Allied, 1, {}});
    Shape::StorageComponent storage;
    storage.minerals = 100.0f;
    storage.sciencePoints = 150.0f;
    world.AddComponent(civ_entity, storage);

    // 2. Advance construction and civilization technology processing
    for (int t = 0; t < 10; ++t) {
        building_sys.update(ctx);
    }
    civ_sys.Update(world, 1.0f);

    const auto& updated_farm = world.GetComponent<Shape::Simulation::BuildingComponent>(farm);
    REQUIRE(updated_farm.construction_progress > 0.5f);

    // 3. Verify technology unlocked when science points reached
    const auto& civ = world.GetComponent<Shape::CivilizationComponent>(civ_entity);
    const auto& store = world.GetComponent<Shape::StorageComponent>(civ_entity);

    REQUIRE(civ.HasTechnology(1) == true);
    REQUIRE(store.sciencePoints == Catch::Approx(50.0f));
}

TEST_CASE("User Scenario 3: Player Controls & Viewport Camera Tracking", "[Scenario][Controls]") {
    Shape::World world;
    Shape::SimulationClock clock(60);
    Shape::SimulationScheduler scheduler;
    Shape::Simulation::SimulationController sim_ctrl(&clock, &scheduler);

    Shape::Rendering::CameraController camera;
    Shape::Rendering::GameInputController input_ctrl(&camera, &sim_ctrl);

    Shape::Math::Vector2f screen_size{1920.0f, 1080.0f};

    // 1. Create player target entity
    Shape::Entity player = world.CreateEntity();
    Shape::TransformComponent player_trans;
    player_trans.position = {500.0f, 300.0f};
    world.AddComponent(player, player_trans);

    // 2. Track entity with camera
    camera.FollowEntity(player);
    camera.Update(&world, 1.0f); // Fast forward camera lerp

    REQUIRE(camera.GetPosition().x == Catch::Approx(500.0f));
    REQUIRE(camera.GetPosition().y == Catch::Approx(300.0f));

    // 3. WorldToScreen transformation
    Shape::Math::Vector2f screen_pos = camera.WorldToScreen(player_trans.position, screen_size);
    REQUIRE(screen_pos.x == Catch::Approx(1920.0f * 0.5f));
    REQUIRE(screen_pos.y == Catch::Approx(1080.0f * 0.5f));

    // 4. Interactive Simulation Playback Controls
    sim_ctrl.Play();
    REQUIRE(sim_ctrl.IsRunning());

    sim_ctrl.SetSpeedPreset(3); // 5x Speed
    REQUIRE(sim_ctrl.GetTimeScale() == 5.0f);

    input_ctrl.ExecuteAction(Shape::Rendering::InputAction::TogglePause);
    REQUIRE(sim_ctrl.IsPaused());

    // Single tick stepping
    uint64_t start_tick = sim_ctrl.GetCurrentTick();
    input_ctrl.ExecuteAction(Shape::Rendering::InputAction::StepSingleTick);
    REQUIRE(sim_ctrl.GetCurrentTick() == start_tick + 1);
}
