#include <catch2/catch_test_macros.hpp>
#include "Simulation/Evolution/SpeciationSystem.hpp"
#include "ECS2/World2.hpp"

TEST_CASE("SpeciationSystem basics", "[speciation]") {
    Shape::ECS2::World2 world;
    Shape::SpeciationSystem sys;
    sys.set_world(&world);
    
    REQUIRE(sys.priority() == 100);
    REQUIRE(sys.name() == "SpeciationSystem");
}
