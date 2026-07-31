#include <catch2/catch_test_macros.hpp>
#include "Shape/Simulation/Systems/BuildingSystem.h"
#include "ECS/World.hpp"
#include "ECS/StorageComponent.hpp"

TEST_CASE("BuildingSystem - Construction & Resource Yield", "[Simulation][Building]") {
    Shape::World world;
    Shape::Simulation::BuildingSystem building_sys(&world);
    Shape::Simulation::DeterministicRng rng(999);

    SECTION("System metadata") {
        REQUIRE(building_sys.name() == "BuildingSystem");
        REQUIRE(building_sys.priority() == 115);
    }

    SECTION("Building Construction and Passive Farm Yield") {
        Shape::Entity farm = building_sys.construct_building(
            Shape::Simulation::BuildingType::Farm, 1, Shape::Math::Vector2f{10.0f, 20.0f}
        );

        REQUIRE(farm != Shape::INVALID_ENTITY);
        REQUIRE(world.HasComponent<Shape::Simulation::BuildingComponent>(farm));
        REQUIRE(world.HasComponent<Shape::StorageComponent>(farm));

        auto& building = world.GetComponent<Shape::Simulation::BuildingComponent>(farm);
        REQUIRE_FALSE(building.is_operational);
        REQUIRE(building.construction_progress < 1.0f);

        // Update system for ~6 seconds (360 ticks) to complete construction & generate yield
        for (uint64_t t = 1; t <= 360; ++t) {
            Shape::TickContext tick_ctx{t, 1.0 / 60.0, t * 0.016, rng};
            building_sys.update(tick_ctx);
        }

        REQUIRE(building.construction_progress >= 1.0f);
        REQUIRE(building.is_operational);

        auto& storage = world.GetComponent<Shape::StorageComponent>(farm);
        REQUIRE(storage.biomass > 0.0f); // Farm yields biomass passively
    }

    SECTION("Building Repair") {
        Shape::Entity depot = building_sys.construct_building(
            Shape::Simulation::BuildingType::Depot, 1, Shape::Math::Vector2f{0.0f, 0.0f}
        );

        auto& building = world.GetComponent<Shape::Simulation::BuildingComponent>(depot);
        building.health = 20.0f;
        building_sys.repair_building(depot, 50.0f);

        REQUIRE(building.health == 70.0f);
    }
}
