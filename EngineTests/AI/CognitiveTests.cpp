#include <catch2/catch_test_macros.hpp>
#include "ECS/World.hpp"
#include "ECS/MindComponent.hpp"
#include "ECS/LifecycleComponent.hpp"
#include "ECS/InfluenceComponent.hpp"
#include "ECS/SentinelComponent.hpp"
#include "ECS/ResourceComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "Physics/ColliderComponent.hpp"
#include "Physics/SpatialHashGrid.hpp"
#include "AI/CognitiveSystem.hpp"
#include "ECS/TransformComponent.hpp"
#include "Influence/InfluenceSystem.hpp"
#include "Sentinel/SentinelSystem.hpp"

using namespace Shape;
using namespace Shape::Math;

TEST_CASE("Cognitive System Needs and Emotion Decay", "[Cognitive]") {
    Shape::World world;
    CognitiveSystem cogSystem;

    Entity agent = world.CreateEntity();
    world.AddComponent<MindComponent>(agent, MindComponent());
    world.AddComponent<LifecycleComponent>(agent, LifecycleComponent());

    auto& mind = world.GetComponent<MindComponent>(agent);
    auto& life = world.GetComponent<LifecycleComponent>(agent);

    // Initial setups
    life.energy = 60.0f; // Hunger should become 40.0f
    mind.fatigue = 10.0f;
    mind.fear = 80.0f;
    mind.rage = 40.0f;

    SECTION("Tick updates need syncing and decays emotions") {
        cogSystem.Update(world, 1.0f); // 1.0s update

        REQUIRE(NearEquals(mind.hunger, 40.0f));
        REQUIRE(NearEquals(mind.fatigue, 10.8f)); // fatigue + 0.8 * dt
        REQUIRE(NearEquals(mind.fear, 76.0f));    // fear - 4.0 * dt
        REQUIRE(NearEquals(mind.rage, 38.0f));    // rage - 2.0 * dt
    }

    SECTION("Memory decays and gets pruned") {
        mind.AddMemory(Math::Vector2f(10.0f, 10.0f), 50.0f);
        REQUIRE(mind.memories.size() == 1);

        // Advance by 31 seconds to trigger age-decay
        cogSystem.Update(world, 31.0f);
        // Strength should have dropped: 50.0 - 0.1 * (31 - 30) = 50.0 - 0.1 = 49.9f
        REQUIRE(mind.memories[0].strength < 50.0f);
        REQUIRE(mind.memories[0].age >= 31.0f);
    }
}

TEST_CASE("Spatial Influence Fields Attenuation", "[Cognitive]") {
    Shape::World world;
    SpatialHashGrid grid(10.0f);
    InfluenceSystem infSystem;

    Entity receiver = world.CreateEntity();
    world.AddComponent<MindComponent>(receiver, MindComponent());
    world.AddComponent<RigidBodyComponent>(receiver, RigidBodyComponent());
    world.AddComponent<TransformComponent>(receiver, TransformComponent());

    Entity emitter = world.CreateEntity();
    world.AddComponent<InfluenceComponent>(emitter, InfluenceComponent(InfluenceType::Fear, 100.0f, 50.0f));
    world.AddComponent<RigidBodyComponent>(emitter, RigidBodyComponent());
    world.AddComponent<TransformComponent>(emitter, TransformComponent());

    auto& rTrans = world.GetComponent<TransformComponent>(receiver);
    
    rTrans.position = Math::Vector2f(0.0f, 0.0f);

    auto& eTrans = world.GetComponent<TransformComponent>(emitter);
    
    eTrans.position = Math::Vector2f(3.0f, 4.0f); // Distance is exactly 5.0 units

    // Populate Spatial Grid
    grid.Insert(receiver, rTrans.position, 2.0f);
    grid.Insert(emitter, eTrans.position, 2.0f);
    

    SECTION("Fear field propagates with quadratic distance falloff") {
        auto& mind = world.GetComponent<MindComponent>(receiver);
        mind.fear = 0.0f;

        infSystem.Update(world, grid);

        // Distance = 5.0 -> distance squared = 25.0
        // Expected value: strength / (1.0 + d^2) = 100.0 / (1.0 + 25.0) = 100.0 / 26.0 = 3.846f
        REQUIRE(NearEquals(mind.accumulatedFearField, 3.846f, 0.01f));
        // fear emotion should have increased accordingly
        REQUIRE(mind.fear > 0.0f);
    }
}

TEST_CASE("Sentinel Knowledge Network Broadcast", "[Cognitive]") {
    Shape::World world;
    SpatialHashGrid grid(10.0f);
    SentinelSystem sentSystem;

    // 1. Create Sentinel
    Entity sentinel = world.CreateEntity();
    world.AddComponent<SentinelComponent>(sentinel, SentinelComponent(SentinelType::Science, 40.0f));
    world.AddComponent<RigidBodyComponent>(sentinel, RigidBodyComponent());
    world.AddComponent<TransformComponent>(sentinel, TransformComponent());
    auto& sTrans = world.GetComponent<TransformComponent>(sentinel);
    
    sTrans.position = Math::Vector2f(0.0f, 0.0f);

    // 2. Create local Shape mind
    Entity shape = world.CreateEntity();
    world.AddComponent<MindComponent>(shape, MindComponent());
    world.AddComponent<RigidBodyComponent>(shape, RigidBodyComponent());
    world.AddComponent<TransformComponent>(shape, TransformComponent());
    auto& shTrans = world.GetComponent<TransformComponent>(shape);
    
    shTrans.position = Math::Vector2f(5.0f, 5.0f); // within radius

    // 3. Create Resource node
    Entity resource = world.CreateEntity();
    world.AddComponent<ResourceComponent>(resource, ResourceComponent());
    world.AddComponent<RigidBodyComponent>(resource, RigidBodyComponent());
    world.AddComponent<TransformComponent>(resource, TransformComponent());
    auto& rTrans = world.GetComponent<TransformComponent>(resource);
    
    rTrans.position = Math::Vector2f(10.0f, 10.0f); // within radius

    auto& res = world.GetComponent<ResourceComponent>(resource);
    res.resourceAmount = 45.0f;

    // Load spatial grid
    grid.Insert(sentinel, sTrans.position, 2.0f);
    
    grid.Insert(shape, shTrans.position, 2.0f);
    grid.Insert(resource, rTrans.position, 2.0f);

    SECTION("Sentinel attaches influence and shares memory locations") {
        // Run Sentinel System update
        sentSystem.Update(world, grid, 0.6f); // forces sync timer check (syncInterval = 0.5f)

        // Validate that Sentinel automatically attached InfluenceComponent
        REQUIRE(world.HasComponent<InfluenceComponent>(sentinel));
        auto& inf = world.GetComponent<InfluenceComponent>(sentinel);
        REQUIRE(inf.type == InfluenceType::Knowledge);
        REQUIRE(NearEquals(inf.strength, 25.0f));

        // Validate shape memory sync
        auto& mind = world.GetComponent<MindComponent>(shape);
        REQUIRE(mind.memories.size() == 1);
        REQUIRE(mind.memories[0].position.x == 10.0f);
        REQUIRE(mind.memories[0].position.y == 10.0f);
        REQUIRE(NearEquals(mind.memories[0].strength, 45.0f));
    }
}
