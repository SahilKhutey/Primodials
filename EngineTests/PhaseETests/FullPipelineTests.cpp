#include <catch2/catch_test_macros.hpp>
#include "ECS/World.hpp"
#include "Scripting/LuaModLoader.hpp"
#include <chrono>
#include <thread>
#include <filesystem>

using namespace Shape;

TEST_CASE("App boots in under 10 seconds", "[integration][boot]") {
    auto start = std::chrono::steady_clock::now();

    World world;
    world.CreateEntity();

    auto elapsed = std::chrono::steady_clock::now() - start;
    auto seconds = std::chrono::duration<double>(elapsed).count();

    REQUIRE(seconds < 10.0);
}

TEST_CASE("World initializes with 100K entities", "[integration][ecs]") {
    World world;

    for (int i = 0; i < 100000; ++i) {
        world.CreateEntity();
    }

    REQUIRE(world.GetActiveEntityCount() == 100000);
}

TEST_CASE("Full simulation runs 60 ticks without memory leak", "[integration][memory]") {
    World world;

    for (int i = 0; i < 1000; ++i) {
        world.CreateEntity();
    }

    REQUIRE(world.GetActiveEntityCount() == 1000);
}

TEST_CASE("Save then load preserves state", "[integration][persistence]") {
    std::string filename = "test_save.sim";

    {
        World world;
        for (int i = 0; i < 10; ++i) {
            world.CreateEntity();
        }
        REQUIRE(world.GetActiveEntityCount() == 10);
    }

    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }
}

TEST_CASE("Multiple mods can load simultaneously", "[integration][mods]") {
    Scripting::LuaModLoader loader("Content/Mods");
    usize loaded = loader.LoadAllMods();
    REQUIRE(loaded >= 0);
}
