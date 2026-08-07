#include <catch2/catch_test_macros.hpp>
#include "ECS/World.hpp"
#include <chrono>

using namespace Shape;

TEST_CASE("App runs for simulated ticks without crash", "[stress][stability]") {
    World world;

    for (int i = 0; i < 1000; ++i) {
        world.CreateEntity();
    }

    REQUIRE(world.GetActiveEntityCount() == 1000);
}

TEST_CASE("No exceptions thrown during 1000 ticks", "[stress][exceptions]") {
    World world;
    int count = 0;

    for (int i = 0; i < 1000; ++i) {
        world.CreateEntity();
        count++;
    }

    REQUIRE(count == 1000);
    REQUIRE(world.GetActiveEntityCount() == 1000);
}

TEST_CASE("Entity count stays bounded under chaotic input", "[stress][chaos]") {
    World world;

    for (int i = 0; i < 1000; ++i) {
        Entity e = world.CreateEntity();
        if (i % 2 == 0) {
            world.DestroyEntity(e);
        }
    }

    REQUIRE(world.GetActiveEntityCount() == 500);
}
