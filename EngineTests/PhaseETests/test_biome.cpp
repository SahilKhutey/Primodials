#include <catch2/catch_test_macros.hpp>
#include "Shape/Simulation/Systems/BiomeSystem.h"

TEST_CASE("BiomeSystem - Spatial Biome Mapping & Modifiers", "[Simulation][Biome]") {
    Shape::Simulation::BiomeSystem biome_sys;

    SECTION("System metadata") {
        REQUIRE(biome_sys.name() == "BiomeSystem");
        REQUIRE(biome_sys.priority() == -35);
    }

    SECTION("Spatial Biome Queries") {
        Shape::Math::Vector2f pos1{0.0f, 0.0f};
        Shape::Math::Vector2f pos2{500.0f, 500.0f};

        Shape::Simulation::BiomeProperties props1 = biome_sys.get_properties_at(pos1);
        Shape::Simulation::BiomeProperties props2 = biome_sys.get_properties_at(pos2);

        REQUIRE_FALSE(props1.name.empty());
        REQUIRE_FALSE(props2.name.empty());
        REQUIRE(props1.fertility_multiplier > 0.0f);
        REQUIRE(props1.movement_friction > 0.0f);
    }

    SECTION("Biome Enum Property Lookup") {
        auto forest_props = Shape::Simulation::BiomeSystem::get_biome_properties(Shape::Simulation::BiomeType::LushForest);
        auto desert_props = Shape::Simulation::BiomeSystem::get_biome_properties(Shape::Simulation::BiomeType::AridDesert);

        REQUIRE(forest_props.fertility_multiplier > desert_props.fertility_multiplier);
        REQUIRE(desert_props.temperature_offset > forest_props.temperature_offset);
    }
}
