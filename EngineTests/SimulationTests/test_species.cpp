/// @file test_species.cpp — SpeciesSchema layer tests

#include <catch2/catch_test_macros.hpp>
#include "ECS/SpeciesSchema.hpp"
#include <cmath>

using namespace Shape;

TEST_CASE("GeometryLayer — RecomputeDerived: triangle", "[species][geometry]") {
    GeometryLayer g;
    g.vertexCount  = 3;
    g.circumradius = 10.0f;
    g.RecomputeDerived();

    // Triangle: interior angle = 60°
    REQUIRE(std::abs(g.interiorAngle - 60.0f) < 0.01f);

    // edge_length = 2 * R * sin(π/3) = 2 * 10 * 0.866 ≈ 17.32
    REQUIRE(std::abs(g.edgeLength - 17.32f) < 0.5f);

    // area = 0.5 * n * R^2 * sin(2π/n) = 0.5 * 3 * 100 * sin(120°) ≈ 129.9
    REQUIRE(std::abs(g.area - 129.9f) < 2.0f);
}

TEST_CASE("GeometryLayer — RecomputeDerived: hexagon", "[species][geometry]") {
    GeometryLayer g;
    g.vertexCount  = 6;
    g.circumradius = 10.0f;
    g.RecomputeDerived();

    // Hexagon: interior angle = 120°
    REQUIRE(std::abs(g.interiorAngle - 120.0f) < 0.01f);

    // edge_length = 2 * 10 * sin(π/6) = 10.0
    REQUIRE(std::abs(g.edgeLength - 10.0f) < 0.1f);

    // More sides → larger area (for same circumradius)
    GeometryLayer tri;
    tri.vertexCount = 3; tri.circumradius = 10.0f; tri.RecomputeDerived();
    REQUIRE(g.area > tri.area);
}

TEST_CASE("Species — default layers have sane values", "[species]") {
    Species sp;
    sp.id = 1;

    REQUIRE(sp.biology.maxHealth > 0.0f);
    REQUIRE(sp.biology.metabolismRate > 0.0f);
    REQUIRE(sp.biology.lifespan > 0.0f);
    REQUIRE(sp.reproduction.fertilityBaseline >= 0.0f);
    REQUIRE(sp.reproduction.fertilityBaseline <= 1.0f);
    REQUIRE(sp.genetics.mutationRate >= 0.0f);
    REQUIRE(sp.genetics.mutationRate <= 1.0f);
}

TEST_CASE("Species — triangle has more aggression than hexagon", "[species]") {
    Species triangle;
    triangle.psychology.aggression = 0.85f;

    Species hexagon;
    hexagon.psychology.aggression = 0.15f;

    REQUIRE(triangle.psychology.aggression > hexagon.psychology.aggression);
}

TEST_CASE("Species — hexagon has highest intelligence", "[species]") {
    Species tri, quad, pent, hex;
    tri.intelligence.rawIq  = 35.0f;
    quad.intelligence.rawIq = 55.0f;
    pent.intelligence.rawIq = 70.0f;
    hex.intelligence.rawIq  = 120.0f;

    REQUIRE(hex.intelligence.rawIq > pent.intelligence.rawIq);
    REQUIRE(pent.intelligence.rawIq > quad.intelligence.rawIq);
    REQUIRE(quad.intelligence.rawIq > tri.intelligence.rawIq);
}

TEST_CASE("Species — metabolism * lifespan yields total energy spend", "[species]") {
    Species sp;
    sp.biology.metabolismRate = 1.0f;
    sp.biology.lifespan       = 4800.0f;
    // Total energy cost of a lifetime = metabolism * lifespan (per tick)
    const float lifetime_cost = sp.biology.metabolismRate * sp.biology.lifespan;
    REQUIRE(lifetime_cost == 4800.0f);
}

TEST_CASE("SpeciesIdComponent — default is INVALID", "[species]") {
    SpeciesIdComponent c;
    REQUIRE(c.id == INVALID_SPECIES_ID);
}
