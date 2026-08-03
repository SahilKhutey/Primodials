#include <catch2/catch_test_macros.hpp>
#include "Simulation/Snapshots/SimulationSnapshot.hpp"
#include "ECS2/World2.hpp"
#include "Simulation/SimulationState.hpp"
#include "ECS2/ComponentRegistry.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include "Simulation/Components/LineageComponents.hpp"
#include "Simulation/Components/ResourceComponents.hpp"
#include <iostream>

struct RegisterSnapshotComponents {
    RegisterSnapshotComponents() {
        SHAPE_REGISTER_COMPONENT(Shape::Position);
        SHAPE_REGISTER_COMPONENT(Shape::Velocity);
        SHAPE_REGISTER_COMPONENT(Shape::Rotation);
        SHAPE_REGISTER_COMPONENT(Shape::SpatialTag);
        SHAPE_REGISTER_COMPONENT(Shape::CreatureState);
        SHAPE_REGISTER_COMPONENT(Shape::ReproductionState);
        SHAPE_REGISTER_COMPONENT(Shape::CreatureGenome);
        SHAPE_REGISTER_COMPONENT(Shape::FitnessScore);
        SHAPE_REGISTER_COMPONENT(Shape::AIState);
        SHAPE_REGISTER_COMPONENT(Shape::FoodDeposit);
        SHAPE_REGISTER_COMPONENT(Shape::FoodTag);
        SHAPE_REGISTER_COMPONENT(Shape::SpeciesIdComponent);
        SHAPE_REGISTER_COMPONENT(Shape::DerivedAttributes);
        SHAPE_REGISTER_COMPONENT(Shape::CombatStats);
        SHAPE_REGISTER_COMPONENT(Shape::CombatTracker);
        SHAPE_REGISTER_COMPONENT(Shape::MatingStatus);
        SHAPE_REGISTER_COMPONENT(Shape::Ancestors);
        SHAPE_REGISTER_COMPONENT(Shape::ColorOverride);
    }
};
static RegisterSnapshotComponents _reg;

TEST_CASE("SimulationSnapshot basics", "[snapshot]") {
    Shape::ECS2::World2 world;
    world.set_resource(Shape::WorldState{});
    
    // Save to memory
    std::vector<u8> blob = Shape::Simulation::SimulationSnapshot::save_to_memory(world);
    REQUIRE(!blob.empty());
    
    // Load from memory
    Shape::ECS2::World2 world2;
    bool success = Shape::Simulation::SimulationSnapshot::load_from_memory(world2, blob);
    REQUIRE(success);
}



TEST_CASE("SimulationSnapshot ID preservation", "[snapshot]") {
    Shape::ECS2::World2 world;
    world.set_resource(Shape::WorldState{});

    auto e1 = world.create();
    world.add<Shape::Position>(e1, Shape::Position{ {10.0f, 20.0f} });
    world.add<Shape::Velocity>(e1, Shape::Velocity{ {1.0f, 2.0f} });

    auto e2 = world.create();
    world.add<Shape::Position>(e2, Shape::Position{ {30.0f, 40.0f} });

    // Save to memory
    std::vector<u8> blob = Shape::Simulation::SimulationSnapshot::save_to_memory(world);
    REQUIRE(!blob.empty());

    // Destroy and recreate to advance generations
    world.destroy(e1);
    world.destroy(e2);
    auto dummy1 = world.create();
    auto dummy2 = world.create();
    (void)dummy1;
    (void)dummy2;

    // Load from memory back into the world
    bool success = Shape::Simulation::SimulationSnapshot::load_from_memory(world, blob);
    REQUIRE(success);

    // Verify EntityIds and components match exactly
    REQUIRE(world.valid(e1));
    REQUIRE(world.valid(e2));
    REQUIRE(world.has<Shape::Position>(e1));
    REQUIRE(world.get<Shape::Position>(e1).value.x == 10.0f);
    REQUIRE(world.get<Shape::Position>(e1).value.y == 20.0f);
    REQUIRE(world.has<Shape::Velocity>(e1));
    REQUIRE(world.get<Shape::Velocity>(e1).value.x == 1.0f);
    REQUIRE(world.get<Shape::Velocity>(e1).value.y == 2.0f);

    REQUIRE(world.has<Shape::Position>(e2));
    REQUIRE(world.get<Shape::Position>(e2).value.x == 30.0f);
    REQUIRE(world.get<Shape::Position>(e2).value.y == 40.0f);
}
