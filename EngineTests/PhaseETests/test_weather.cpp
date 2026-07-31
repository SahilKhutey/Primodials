#include <catch2/catch_test_macros.hpp>
#include "Shape/Simulation/Systems/WeatherSystem.h"

TEST_CASE("WeatherSystem - Season Cycles & Determinism", "[Simulation][Weather]") {
    Shape::Simulation::WeatherSystem weather(100); // 100 ticks per season
    Shape::Simulation::DeterministicRng rng(12345);

    SECTION("Initial Spring state") {
        REQUIRE(weather.name() == "WeatherSystem");
        REQUIRE(weather.priority() == -40);
        REQUIRE(weather.current_season() == Shape::Simulation::Season::Spring);
        REQUIRE(weather.temperature() > 0.0f);
    }

    SECTION("Season progression over ticks") {
        // Advance to Summer (tick 105)
        for (uint64_t t = 1; t <= 105; ++t) {
            Shape::TickContext tick_ctx{t, 1.0 / 60.0, t * 0.016, rng};
            weather.update(tick_ctx);
        }
        REQUIRE(weather.current_season() == Shape::Simulation::Season::Summer);

        // Advance to Autumn (tick 205)
        for (uint64_t t = 106; t <= 205; ++t) {
            Shape::TickContext tick_ctx{t, 1.0 / 60.0, t * 0.016, rng};
            weather.update(tick_ctx);
        }
        REQUIRE(weather.current_season() == Shape::Simulation::Season::Autumn);

        // Advance to Winter (tick 305)
        for (uint64_t t = 206; t <= 305; ++t) {
            Shape::TickContext tick_ctx{t, 1.0 / 60.0, t * 0.016, rng};
            weather.update(tick_ctx);
        }
        REQUIRE(weather.current_season() == Shape::Simulation::Season::Winter);
    }

    SECTION("Forced season and weather overrides") {
        weather.force_season(Shape::Simulation::Season::Winter);
        weather.force_weather(Shape::Simulation::WeatherType::Blizzard);

        REQUIRE(weather.current_season() == Shape::Simulation::Season::Winter);
        REQUIRE(weather.current_weather() == Shape::Simulation::WeatherType::Blizzard);
        REQUIRE(weather.state().metabolic_drain_multiplier > 1.0f);
        REQUIRE(weather.state().resource_growth_multiplier < 0.5f);
    }
}
