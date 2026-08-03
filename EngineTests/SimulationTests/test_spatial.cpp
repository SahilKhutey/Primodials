/// @file test_spatial.cpp — SpatialGrid tests

#include <catch2/catch_test_macros.hpp>
#include "Simulation/Spatial/SpatialGrid.hpp"
#include "Simulation/SimulationState.hpp"
#include "Math/Include/ShapeEngine/Math/Vector2.hpp"
#include <cmath>
#include <algorithm>

using namespace Shape;
using V2 = Math::Vector2f;

static SpatialGrid make_grid(float cell = 20.0f, bool wrap = false) {
    SpatialGrid::Config cfg;
    cfg.cell_size = cell;
    cfg.bounds.min = {-200.0f, -200.0f};
    cfg.bounds.max = { 200.0f,  200.0f};
    cfg.wrap = wrap;
    return SpatialGrid(cfg);
}

TEST_CASE("SpatialGrid — insert and entity_count", "[spatial]") {
    auto grid = make_grid();
    grid.insert(1, {10.0f, 10.0f});
    grid.insert(2, {50.0f, 50.0f});
    REQUIRE(grid.entity_count() == 2);
}

TEST_CASE("SpatialGrid — query_circle returns all nearby", "[spatial]") {
    auto grid = make_grid();
    grid.insert(1, V2{0.0f, 0.0f});
    grid.insert(2, V2{5.0f, 0.0f});
    grid.insert(3, V2{100.0f, 100.0f}); // Far away

    std::vector<u32> out;
    grid.query_circle(V2{0.0f, 0.0f}, 20.0f, out);

    REQUIRE(std::find(out.begin(), out.end(), 3u) == out.end());
    REQUIRE(std::find(out.begin(), out.end(), 1u) != out.end());
    REQUIRE(std::find(out.begin(), out.end(), 2u) != out.end());
}

TEST_CASE("SpatialGrid — query_circle with large radius returns everything", "[spatial]") {
    auto grid = make_grid();
    for (u32 i = 0; i < 10; ++i)
        grid.insert(i, V2{static_cast<float>(i * 30) - 150.0f, 0.0f});

    std::vector<u32> out;
    grid.query_circle(V2{0.0f, 0.0f}, 300.0f, out);
    REQUIRE(out.size() >= 10);
}

TEST_CASE("SpatialGrid — remove decrements entity_count", "[spatial]") {
    auto grid = make_grid();
    grid.insert(1, V2{0.0f, 0.0f});
    grid.insert(2, V2{0.0f, 0.0f});
    grid.remove(1, V2{0.0f, 0.0f});
    REQUIRE(grid.entity_count() == 1);
}

TEST_CASE("SpatialGrid — update moves entity to new cell", "[spatial]") {
    auto grid = make_grid(20.0f, false);
    grid.insert(1, V2{0.0f, 0.0f});

    std::vector<u32> out1;
    grid.query_circle(V2{0.0f, 0.0f}, 15.0f, out1);
    REQUIRE(std::find(out1.begin(), out1.end(), 1u) != out1.end());

    grid.update(1, V2{0.0f, 0.0f}, V2{150.0f, 150.0f});

    std::vector<u32> out2;
    grid.query_circle(V2{0.0f, 0.0f}, 15.0f, out2);
    REQUIRE(std::find(out2.begin(), out2.end(), 1u) == out2.end());

    std::vector<u32> out3;
    grid.query_circle(V2{150.0f, 150.0f}, 15.0f, out3);
    REQUIRE(std::find(out3.begin(), out3.end(), 1u) != out3.end());
}

TEST_CASE("SpatialGrid — wrapping grid finds entity across boundary", "[spatial]") {
    auto grid = make_grid(20.0f, true);
    grid.insert(1, V2{195.0f, 0.0f});

    std::vector<u32> out;
    grid.query_circle(V2{-195.0f, 0.0f}, 25.0f, out);
    REQUIRE(std::find(out.begin(), out.end(), 1u) != out.end());
}

TEST_CASE("SpatialGrid — clear empties all cells", "[spatial]") {
    auto grid = make_grid();
    for (u32 i = 0; i < 50; ++i) grid.insert(i, V2{0, 0});
    REQUIRE(grid.entity_count() == 50);
    grid.clear();
    REQUIRE(grid.entity_count() == 0);
    REQUIRE(grid.occupied_cells() == 0);
}

TEST_CASE("SpatialGrid — stats density", "[spatial]") {
    auto grid = make_grid();
    for (u32 i = 0; i < 20; ++i) grid.insert(i, V2{0.0f, 0.0f});
    auto s = grid.stats();
    REQUIRE(s.entity_count == 20);
    REQUIRE(s.max_density == 20);
}

TEST_CASE("WorldBounds — wrap keeps point inside", "[spatial][worldbounds]") {
    WorldBounds b;
    b.min = {-100.0f, -100.0f};
    b.max = { 100.0f,  100.0f};

    V2 p{105.0f, -110.0f};
    b.wrap(p);
    REQUIRE(p.x >= b.min.x); REQUIRE(p.x < b.max.x);
    REQUIRE(p.y >= b.min.y); REQUIRE(p.y < b.max.y);
}

TEST_CASE("WorldBounds — delta_wrapped gives shortest path", "[spatial][worldbounds]") {
    WorldBounds b;
    b.min = {0.0f, 0.0f};
    b.max = {100.0f, 100.0f};

    const V2 d = b.delta_wrapped(V2{5.0f, 0.0f}, V2{95.0f, 0.0f});
    REQUIRE(std::abs(d.x) < 15.0f);
}
