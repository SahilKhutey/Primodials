#include <catch2/catch_all.hpp>
#include "Core/Bootstrap.hpp"

TEST_CASE("Engine Bootstrap Lifecycle", "[Bootstrap]") {
    shape::Engine& engine = shape::Engine::Get();
    engine.Shutdown(); // Ensure clean state since it's a singleton

    REQUIRE_FALSE(engine.IsRunning());

    SECTION("Initialization starts the engine loop") {
        REQUIRE(engine.Initialize());
        REQUIRE(engine.IsRunning());

        engine.Run(10);
        REQUIRE(engine.IsRunning()); // Should still be running after 10 loops

        engine.Shutdown();
        REQUIRE_FALSE(engine.IsRunning());
    }
}
