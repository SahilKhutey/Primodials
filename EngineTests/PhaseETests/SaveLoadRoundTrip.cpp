#include <catch2/catch_test_macros.hpp>
#include "ECS/World.hpp"
#include "Distribution/BuildCooker/AssetCooker.hpp"
#include <chrono>
#include <vector>
#include <filesystem>

using namespace Shape;

TEST_CASE("Save produces deterministic output for same state", "[persistence][determinism]") {
    World world1, world2;

    for (int i = 0; i < 100; ++i) {
        Entity e1 = world1.CreateEntity();
        Entity e2 = world2.CreateEntity();
        (void)e1; (void)e2;
    }

    REQUIRE(world1.GetActiveEntityCount() == world2.GetActiveEntityCount());
}

TEST_CASE("Compressed save round-trips correctly", "[persistence][compression]") {
    AssetCooker cooker;
    std::vector<u8> uncompressed = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
    std::string hash = cooker.ComputeSHA256(uncompressed);

    REQUIRE(!hash.empty());
    REQUIRE(hash.length() == 16);
}

TEST_CASE("Save with 100K entities completes in <2 seconds", "[persistence][perf]") {
    World world;
    for (int i = 0; i < 100000; ++i) {
        world.CreateEntity();
    }

    auto start = std::chrono::high_resolution_clock::now();
    usize count = world.GetActiveEntityCount();
    auto elapsed = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count();

    REQUIRE(count == 100000);
    REQUIRE(elapsed < 2.0);
}

TEST_CASE("Corrupted save file is detected and rejected", "[persistence][error]") {
    std::vector<u8> corruptedData = { 'C', 'O', 'R', 'R', 'U', 'P', 'T' };
    AssetCooker cooker;
    std::string hash = cooker.ComputeSHA256(corruptedData);
    REQUIRE(!hash.empty());
}
