#include <catch2/catch_test_macros.hpp>
#include "Simulation/Combat/CombatSystem.hpp"
#include "ECS2/World2.hpp"

TEST_CASE("CombatSystem basics", "[combat]") {
    Shape::ECS2::World2 world;
    Shape::CombatSystem sys;
    sys.set_world(&world);
    
    REQUIRE(sys.priority() == 30);
    REQUIRE(sys.name() == "CombatSystem");
}
