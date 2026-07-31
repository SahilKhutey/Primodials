#include <catch2/catch_test_macros.hpp>
#include "Shape/Simulation/Systems/ProceduralWorldSystem.h"

TEST_CASE("ProceduralWorldSystem - Initialization and Execution", "[Simulation][Procedural]") {
    Shape::Simulation::ProceduralWorldSystem proc_sys;
    Shape::Simulation::DeterministicRng rng(42);
    Shape::TickContext ctx{1, 1.0 / 60.0, 0.016, rng};

    SECTION("System metadata") {
        REQUIRE(proc_sys.name() == "ProceduralWorldSystem");
        REQUIRE(proc_sys.priority() == 113);
    }

    SECTION("System update loop") {
        REQUIRE_NOTHROW(proc_sys.update(ctx));
    }
}
