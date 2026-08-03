#include <catch2/catch_test_macros.hpp>
#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include "Simulation/ReplaySystem.hpp"
#include "Serialization/Serializer.hpp"
#include "Utility/CRC32.hpp"
#include <sstream>

using namespace Shape;

TEST_CASE("Serialization System - Save/Load Maintains Determinism", "[Serialization]") {
    World originalWorld;

    // Populate world with deterministic entities
    for (int i = 0; i < 1000; ++i) {
        Entity e = originalWorld.CreateEntity();
        originalWorld.AddComponent(e, TransformComponent{
            .position = Math::Vector2f(static_cast<f32>(i) * 1.5f, static_cast<f32>(i) * -0.5f),
            .rotation = static_cast<f32>(i % 360),
            .scale = Math::Vector2f(1.0f, 1.0f)
        });
    }

    // Capture baseline hash
    u32 baselineHash = 0;
    auto originalEntities = originalWorld.Query<TransformComponent>();
    for (Entity e : originalEntities) {
        auto& t = originalWorld.GetComponent<TransformComponent>(e);
        baselineHash = Utility::CRC32::Calculate(&t.position, sizeof(t.position), baselineHash);
    }

    // Serialize
    std::ostringstream oss(std::ios::binary);
    REQUIRE(Serializer::SaveWorld(originalWorld, oss));

    // Deserialize into a new World
    World loadedWorld;
    std::istringstream iss(oss.str(), std::ios::binary);
    REQUIRE(Serializer::LoadWorld(loadedWorld, iss));

    // Verify Active Entity Count
    REQUIRE(loadedWorld.GetActiveEntityCount() == originalWorld.GetActiveEntityCount());

    // Capture loaded hash
    u32 loadedHash = 0;
    auto loadedEntities = loadedWorld.Query<TransformComponent>();
    for (Entity e : loadedEntities) {
        auto& t = loadedWorld.GetComponent<TransformComponent>(e);
        loadedHash = Utility::CRC32::Calculate(&t.position, sizeof(t.position), loadedHash);
    }

    // Verify deterministic state
    REQUIRE(baselineHash == loadedHash);
}

TEST_CASE("ReplaySystem - Keyframe Snapshotting", "[Serialization]") {
    World world;
    Entity e = world.CreateEntity();
    world.AddComponent(e, TransformComponent{ .position = Math::Vector2f(10.0f, 20.0f) });

    // Capture keyframe at tick 100
    ReplaySystem::ClearKeyframes();
    ReplaySystem::CaptureKeyframe(world, 100);

    REQUIRE(ReplaySystem::GetKeyframeCount() == 1);

    // Modify world state
    world.GetComponent<TransformComponent>(e).position.x = 999.0f;
    REQUIRE(world.GetComponent<TransformComponent>(e).position.x == 999.0f);

    // Restore keyframe
    REQUIRE(ReplaySystem::RestoreKeyframe(world, 100));

    // Verify state was correctly restored
    REQUIRE(world.GetComponent<TransformComponent>(e).position.x == 10.0f);
}
