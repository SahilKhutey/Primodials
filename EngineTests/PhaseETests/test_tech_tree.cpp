#include <catch2/catch_test_macros.hpp>
#include "Shape/Simulation/Systems/TechnologySystem.h"

TEST_CASE("TechnologySystem - Initialization and Execution", "[Simulation][Technology]") {
    Shape::Simulation::TechnologySystem tech_sys;
    Shape::Simulation::DeterministicRng rng(42);
    Shape::TickContext ctx{1, 1.0 / 60.0, 0.016, rng};

    SECTION("System metadata") {
        REQUIRE(tech_sys.name() == "TechnologySystem");
        REQUIRE(tech_sys.priority() == 110);
    }

    SECTION("System update loop") {
        REQUIRE_NOTHROW(tech_sys.update(ctx));
    }
}
