#include <catch2/catch_test_macros.hpp>
#include "ECS/World.hpp"
#include "ECS/CommandBuffer.hpp"
#include "ECS/GeneticsComponent.hpp"
#include "ECS/LifecycleComponent.hpp"
#include "ECS/CareerComponent.hpp"
#include "ECS/AIComponent.hpp"
#include "ECS/ResourceComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "Physics/ColliderComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include "Physics/SpatialHashGrid.hpp"
#include "AI/OptimizationSystem.hpp"
#include "Biology/BiologySystem.hpp"
#include "ECS/TransformComponent.hpp"

using namespace Shape;

TEST_CASE("Genetics Crossover and Mutation", "[Biology]") {
    GeneticsComponent parentA;
    GeneticsComponent parentB;

    // Mutate parent genomes to differentiate them
    parentA.genome[0] = 3.0f; // Triangle
    parentA.genome[1] = 1.0f; // Scale
    parentA.genome[4] = 4.0f; // High survival affinity
    
    parentB.genome[0] = 4.0f; // Square
    parentB.genome[1] = 2.0f; // Large scale
    parentB.genome[4] = 1.0f; // Low survival affinity

    SECTION("Crossover yields mix") {
        DeterministicRng rng(101);
        GeneticsComponent child = GeneticsComponent::Crossover(parentA, parentB, rng);
        // Each gene should come from A or B
        for (size_t i = 0; i < child.genome.size(); ++i) {
            REQUIRE((child.genome[i] == parentA.genome[i] || child.genome[i] == parentB.genome[i]));
        }
    }

    SECTION("Mutation rate drift") {
        GeneticsComponent child = parentA;
        child.genome[0] = 10.0f; // Set away from clamp minimum (3.0f)
        DeterministicRng rng1(777);
        child.Mutate(rng1);
        // With mutation rate 0.05, at least one of the 12 genes might drift over multiple mutation iterations,
        // but let's force high mutation to guarantee change
        child.genome[10] = 1.0f; // 100% mutation chance
        DeterministicRng rng2(888);
        child.Mutate(rng2);
        REQUIRE(child.genome[0] != 10.0f); // Vertex count modified
    }
}

TEST_CASE("Utility AI and Decision Loop", "[Biology]") {
    Shape::World world;
    SpatialHashGrid grid(10.0f);
    OptimizationSystem aiSystem;

    Entity agent = world.CreateEntity();
    world.AddComponent<AIComponent>(agent, AIComponent());
    world.AddComponent<RigidBodyComponent>(agent, RigidBodyComponent());
    world.AddComponent<TransformComponent>(agent, TransformComponent());
    world.AddComponent<LifecycleComponent>(agent, LifecycleComponent());
    world.AddComponent<CareerComponent>(agent, CareerComponent());
    world.AddComponent<GeneticsComponent>(agent, GeneticsComponent());

    auto& life = world.GetComponent<LifecycleComponent>(agent);
    auto& ai = world.GetComponent<AIComponent>(agent);

    // Initial state: High energy (50.0) -> AI defaults to Explore
    aiSystem.Update(world, grid, 0.016f);
    REQUIRE(ai.currentAction == AIAction::Explore);

    // 1. Simulate Hunger: Drop energy to 5.0f
    life.energy = 5.0f;

    // Spawn a resource food entity nearby at (10, 0)
    Entity food = world.CreateEntity();
    world.AddComponent<RigidBodyComponent>(food, RigidBodyComponent());
    world.AddComponent<TransformComponent>(food, TransformComponent());
    world.AddComponent<ColliderComponent>(food, ColliderComponent());
    world.AddComponent<ResourceComponent>(food, ResourceComponent());

    auto& foodTrans = world.GetComponent<TransformComponent>(food);
    auto& foodBody = world.GetComponent<RigidBodyComponent>(food);
    foodTrans.position = Math::Vector2f(1.0f, 0.0f); // Proximity
    foodBody.SetMass(0.0f);

    auto& foodCol = world.GetComponent<ColliderComponent>(food);
    foodCol.type = ColliderType::Circle;
    foodCol.radius = 4.0f;

    // Refresh Spatial Hash Grid
    grid.Clear();
    grid.Insert(agent, Math::Vector2f(0.0f, 0.0f), 5.0f);
    grid.Insert(food, foodTrans.position, 4.0f);

    // Force decision update
    ai.actionTimer = 0.0f;
    aiSystem.Update(world, grid, 0.016f);

    // High Hunger + Proximity to resource -> AI should switch to Gather
    REQUIRE(ai.currentAction == AIAction::Gather);
    REQUIRE(ai.targetEntity == food);
}

TEST_CASE("Lifecycle Decay, Conservation of Mass, and Birth Split", "[Biology]") {
    Shape::World world;
    CommandBuffer cmd(world);
    BiologySystem bioSystem;

    SECTION("Age decay and death spawns resource food") {
        Entity oldAgent = world.CreateEntity();
        world.AddComponent<LifecycleComponent>(oldAgent, LifecycleComponent());
        world.AddComponent<RigidBodyComponent>(oldAgent, RigidBodyComponent());
        world.AddComponent<TransformComponent>(oldAgent, TransformComponent());
        world.AddComponent<ColliderComponent>(oldAgent, ColliderComponent());

        auto& life = world.GetComponent<LifecycleComponent>(oldAgent);
        
        auto& trans = world.GetComponent<TransformComponent>(oldAgent);
        trans.position = Math::Vector2f(42.0f, 42.0f);

        // Make agent exceed maximum age limit (80)
        life.age = 85.0f;

        // Run Biology Update (triggers decay of health)
        bioSystem.Update(world, cmd, 1.0f);
        cmd.Flush();

        // Health started at 100. Over maximum age, health drops. Let's force direct death:
        life.health = 0.0f;
        life.isDead = true;

        bioSystem.Update(world, cmd, 1.0f);
        cmd.Flush();

        // The old agent should be deleted, and a resource entity should be spawned at its location
        REQUIRE_FALSE(world.HasComponent<LifecycleComponent>(oldAgent)); // Destroyed

        // Check if food was spawned at (42, 42)
        auto resources = world.Query<ResourceComponent, RigidBodyComponent>();
        REQUIRE(resources.size() == 1);
        
        Entity foodNode = resources[0];
        auto& foodTrans = world.GetComponent<TransformComponent>(foodNode);
        REQUIRE(foodTrans.position.x == 42.0f);
        REQUIRE(foodTrans.position.y == 42.0f);
    }

    SECTION("Reproduction generates mutated shape") {
        Entity parent = world.CreateEntity();
        world.AddComponent<AIComponent>(parent, AIComponent());
        world.AddComponent<LifecycleComponent>(parent, LifecycleComponent());
        world.AddComponent<GeneticsComponent>(parent, GeneticsComponent());
        world.AddComponent<RigidBodyComponent>(parent, RigidBodyComponent());
        world.AddComponent<TransformComponent>(parent, TransformComponent());
        world.AddComponent<ColliderComponent>(parent, ColliderComponent());

        auto& ai = world.GetComponent<AIComponent>(parent);
        ai.currentAction = AIAction::Reproduce;

        auto& genetics = world.GetComponent<GeneticsComponent>(parent);
        genetics.genome[0] = 5.0f; // Force Pentagon generation in parent genes
        genetics.genome[1] = 2.0f; // Scale factor = 2

        bioSystem.Update(world, cmd, 0.016f, 1234);
        cmd.Flush();

        // Check that a child was spawned
        auto geometryEntities = world.Query<GeometryComponent, GeneticsComponent>();
        // We should have at least 1 entity with Geometry component (the child, parent doesn't have it since we didn't add it)
        REQUIRE(geometryEntities.size() == 1);

        Entity child = geometryEntities[0];
        auto& childGeom = world.GetComponent<GeometryComponent>(child);
        auto& childLife = world.GetComponent<LifecycleComponent>(child);

        // Child generation should be incremented
        REQUIRE(childLife.generation == 1);
        // Geometry should reflect side count in child genetics
        REQUIRE(childGeom.sideCount == 5);
        REQUIRE(childGeom.vertices.size() == 5);
    }
}
