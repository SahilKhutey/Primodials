#include <catch2/catch_test_macros.hpp>
#include "Simulation/Genome/GenomeDecoder.hpp"
#include "ECS/SpeciesSchema.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include <cmath>

TEST_CASE("GenomeDecoder basics", "[genome]") {
    Shape::CreatureGenome genome;
    // Default 0.5 genes -> 0 delta
    for (int i = 0; i < 16; ++i) genome.genes[i] = 0.5f;

    Shape::Simulation::Genome::DerivedTraits traits = Shape::Simulation::Genome::decode(genome);
    REQUIRE(std::abs(traits.health_delta) < 0.001f);
    REQUIRE(std::abs(traits.speed_delta) < 0.001f);
    
    // Max values (1.0) -> +delta
    for (int i = 0; i < 16; ++i) genome.genes[i] = 1.0f;
    traits = Shape::Simulation::Genome::decode(genome);
    REQUIRE(traits.health_delta > 0.0f);
    REQUIRE(traits.speed_delta > 0.0f);
}
