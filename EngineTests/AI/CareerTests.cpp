#include <catch2/catch_test_macros.hpp>
#include "ECS/World.hpp"
#include "ECS/CareerComponent.hpp"
#include "ECS/AIComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include "ECS/MindComponent.hpp"
#include "ECS/LifecycleComponent.hpp"
#include "ECS/GeneticsComponent.hpp"
#include "ECS/InfluenceComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "Physics/SpatialHashGrid.hpp"
#include "ECS/CareerSystem.hpp"
#include "AI/OptimizationSystem.hpp"
#include "ECS/TransformComponent.hpp"

using namespace Shape;
using namespace Shape::Math;

TEST_CASE("Dynamic Career Skill Progression and Affinity", "[Career]") {
    Shape::World world;
    CareerSystem careerSystem;

    Entity agent = world.CreateEntity();
    world.AddComponent<CareerComponent>(agent, CareerComponent());
    world.AddComponent<AIComponent>(agent, AIComponent());
    world.AddComponent<GeometryComponent>(agent, GeometryComponent(3, 5.0f)); // Triangle shape affinity

    auto& career = world.GetComponent<CareerComponent>(agent);
    auto& ai = world.GetComponent<AIComponent>(agent);

    SECTION("Skill increases based on active tasks") {
        ai.currentAction = AIAction::Gather;
        
        careerSystem.Update(world, 10.0f); // Ticked by 10s -> grows by 10 * 0.1 = 1.0f

        REQUIRE(NearEquals(career.skillSurvival, 2.0f));
        REQUIRE(NearEquals(career.skillExploration, 1.0f)); // Unchanged
    }

    SECTION("Ranks promote automatically") {
        career.skillSurvival = 3.5f;
        careerSystem.Update(world, 0.016f);
        REQUIRE(career.rank == 2); // Specialist (>= 2.5)

        career.skillSurvival = 5.5f;
        careerSystem.Update(world, 0.016f);
        REQUIRE(career.rank == 3); // Expert (>= 5.0)
    }

    SECTION("Vertices trigger shape affinities") {
        careerSystem.Update(world, 0.016f);
        // Triangle shape should boost military interest
        REQUIRE(NearEquals(career.interestMilitary, 2.5f));
    }
}

TEST_CASE("Memory Navigation and Fear Escape Triggers", "[Career]") {
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
    world.AddComponent<MindComponent>(agent, MindComponent());

    auto& mind = world.GetComponent<MindComponent>(agent);
    auto& trans = world.GetComponent<TransformComponent>(agent);
    
    auto& ai = world.GetComponent<AIComponent>(agent);
    auto& life = world.GetComponent<LifecycleComponent>(agent);

    trans.position = Math::Vector2f(0.0f, 0.0f);
    life.energy = 5.0f; // Starving

    SECTION("Navigates to remembered location when food is out of sight") {
        // Place resource memory log at (10, 10) in mind
        mind.AddMemory(Math::Vector2f(10.0f, 10.0f), 50.0f);

        // Grid does not contain food (out of sight)
        grid.Clear();
        grid.Insert(agent, trans.position, 5.0f);

        ai.actionTimer = 0.0f;
        aiSystem.Update(world, grid, 0.016f);

        // AI selects Gather and targetPosition is the remembered food node
        REQUIRE(ai.currentAction == AIAction::Gather);
        REQUIRE(ai.targetPosition.x == 10.0f);
        REQUIRE(ai.targetPosition.y == 10.0f);
    }

    SECTION("Flee from fear emitter overrides hunger") {
        mind.fear = 90.0f; // Terrified

        // Spawn threat emitter nearby at (10, 0) projecting Fear
        Entity threat = world.CreateEntity();
        world.AddComponent<RigidBodyComponent>(threat, RigidBodyComponent());
        world.AddComponent<TransformComponent>(threat, TransformComponent());
        world.AddComponent<InfluenceComponent>(threat, InfluenceComponent(InfluenceType::Fear, 50.0f, 20.0f));

        auto& tTrans = world.GetComponent<TransformComponent>(threat);
    
        tTrans.position = Math::Vector2f(5.0f, 0.0f); // Directly right

        grid.Clear();
        grid.Insert(agent, trans.position, 5.0f);
        grid.Insert(threat, tTrans.position, 5.0f);

        ai.actionTimer = 0.0f;
        aiSystem.Update(world, grid, 0.016f);

        // Escape utility overrides hunger -> switch to Escape
        REQUIRE(ai.currentAction == AIAction::Escape);
        // Target position should point away from threat (to the left)
        REQUIRE(ai.targetPosition.x < 0.0f);
    }
}
