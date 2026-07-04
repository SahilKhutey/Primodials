#include <catch2/catch_test_macros.hpp>
#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include "Utility/CRC32.hpp"
#include "Networking/LockstepSynchronizer.hpp"
#include "Networking/DesyncVerifier.hpp"

using namespace Shape;
using namespace Shape::Utility;

TEST_CASE("Utility: CRC32 string hashing produces stable output", "[networking][crc]") {
    u32 hash1 = CRC32::Calculate("hello world");
    u32 hash2 = CRC32::Calculate("hello world");
    u32 hash3 = CRC32::Calculate("hello world!"); // Different string

    REQUIRE(hash1 == hash2);
    REQUIRE(hash1 != hash3);
}

TEST_CASE("LockstepSynchronizer: Prevents advancement if inputs are missing", "[networking][lockstep]") {
    LockstepSynchronizer sync;
    sync.Initialize(1, {1, 2}); // Players 1 and 2

    // Nobody has sent inputs yet
    REQUIRE(sync.CanAdvanceFrame(0) == false);

    // Player 1 sends input for frame 0
    sync.PushInput(1, PlayerInput{0, {0,0}, 0});
    REQUIRE(sync.CanAdvanceFrame(0) == false);

    // Player 2 sends input for frame 0
    sync.PushInput(2, PlayerInput{0, {0,0}, 0});
    REQUIRE(sync.CanAdvanceFrame(0) == true);
    
    // Nobody sent inputs for frame 1
    REQUIRE(sync.CanAdvanceFrame(1) == false);
}

TEST_CASE("DesyncVerifier: Consistently hashes identical worlds", "[networking][desync]") {
    World world1;
    Entity e1_1 = world1.CreateEntity();
    world1.AddComponent(e1_1, TransformComponent{{10.0f, 5.0f}, 0.0f, {1.0f, 1.0f}});

    World world2;
    Entity e2_1 = world2.CreateEntity();
    world2.AddComponent(e2_1, TransformComponent{{10.0f, 5.0f}, 0.0f, {1.0f, 1.0f}});

    DesyncVerifier verifier;
    u32 hash1 = verifier.CalculateStateHash(world1);
    u32 hash2 = verifier.CalculateStateHash(world2);

    REQUIRE(hash1 == hash2);
    
    // Modify one float slightly
    auto& t = world2.GetComponent<TransformComponent>(e2_1);
    t.position.x = 10.001f;
    u32 hash3 = verifier.CalculateStateHash(world2);

    REQUIRE(hash1 != hash3);
}
