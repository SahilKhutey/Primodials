#include <catch2/catch_test_macros.hpp>
#include "Simulation/Reproduction/ReproductionSystem.hpp"
#include "ECS2/World2.hpp"

TEST_CASE("ReproductionSystem basics", "[reproduction]") {
    Shape::ECS2::World2 world;
    Shape::ReproductionSystem sys;
    sys.set_world(&world);
    
    // Simplistic stub test ensuring it compiles and registers
    REQUIRE(sys.priority() == 0);
    REQUIRE(sys.name() == "ReproductionSystem");
}
