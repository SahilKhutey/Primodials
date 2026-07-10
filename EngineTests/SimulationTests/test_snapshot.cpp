#include <catch2/catch_test_macros.hpp>
#include "Simulation/Snapshots/SimulationSnapshot.hpp"
#include "ECS2/World2.hpp"
#include "Simulation/SimulationState.hpp"

TEST_CASE("SimulationSnapshot basics", "[snapshot]") {
    Shape::ECS2::World2 world;
    world.set_resource(Shape::WorldState{});
    
    // Save to memory
    std::vector<u8> blob = Shape::Simulation::SimulationSnapshot::save_to_memory(world);
    REQUIRE(!blob.empty());
    
    // Load from memory
    Shape::ECS2::World2 world2;
    bool success = Shape::Simulation::SimulationSnapshot::load_from_memory(world2, blob);
    REQUIRE(success);
}
