#include <catch2/catch_test_macros.hpp>
#include "Shape/Simulation/Systems/DiplomacySystem.h"

TEST_CASE("DiplomacySystem - Initialization and Execution", "[Simulation][Diplomacy]") {
    Shape::Simulation::DiplomacySystem diplomacy_sys;
    Shape::Simulation::DeterministicRng rng(42);
    Shape::TickContext ctx{1, 1.0 / 60.0, 0.016, rng};

    SECTION("System properties") {
        REQUIRE(diplomacy_sys.name() == "DiplomacySystem");
        REQUIRE(diplomacy_sys.priority() == 111);
    }

    SECTION("System update loop") {
        REQUIRE_NOTHROW(diplomacy_sys.update(ctx));
    }
}
