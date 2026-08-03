/// @file test_entity.cpp — ECS2 Entity lifecycle tests

#include <catch2/catch_test_macros.hpp>
#include "ECS2/World2.hpp"
#include "ECS2/ComponentRegistry.hpp"

using namespace Shape::ECS2;

// ─── Minimal test components ──────────────────────────────────────────────────

struct Pos2D { float x = 0, y = 0; };
struct Vel2D { float vx = 0, vy = 0; };
struct Mass  { float kg = 1.0f; };

// Auto-registration guard (called once before tests run in this TU)
struct EcsRegistrar {
    EcsRegistrar() {
        SHAPE_REGISTER_COMPONENT(Pos2D);
        SHAPE_REGISTER_COMPONENT(Vel2D);
        SHAPE_REGISTER_COMPONENT(Mass);
    }
};
static EcsRegistrar _reg;

// ─── Tests ───────────────────────────────────────────────────────────────────

TEST_CASE("ECS2 Entity — create returns valid handle", "[ecs2][entity]") {
    World2 world;
    EntityId e = world.create();
    REQUIRE(e.is_valid());
    REQUIRE(world.valid(e));
    REQUIRE(world.entity_count() == 1);
}

TEST_CASE("ECS2 Entity — destroy invalidates handle", "[ecs2][entity]") {
    World2 world;
    EntityId e = world.create();
    world.destroy(e);
    REQUIRE_FALSE(world.valid(e));
    REQUIRE(world.entity_count() == 0);
}

TEST_CASE("ECS2 Entity — destroyed handle is stale", "[ecs2][entity]") {
    World2 world;
    EntityId e1 = world.create();
    world.destroy(e1);
    EntityId e2 = world.create();  // Reuses slot
    // Same index but different generation
    REQUIRE_FALSE(world.valid(e1));
    REQUIRE(world.valid(e2));
    if (e1.index == e2.index)
        REQUIRE(e1.generation != e2.generation);
}

TEST_CASE("ECS2 Entity — add and get component", "[ecs2][entity]") {
    World2 world;
    EntityId e = world.create();
    world.add<Pos2D>(e, {3.0f, 4.0f});
    REQUIRE(world.has<Pos2D>(e));
    REQUIRE(world.get<Pos2D>(e).x == 3.0f);
    REQUIRE(world.get<Pos2D>(e).y == 4.0f);
}

TEST_CASE("ECS2 Entity — remove component", "[ecs2][entity]") {
    World2 world;
    EntityId e = world.create();
    world.add<Pos2D>(e, {1, 2});
    world.add<Vel2D>(e, {5, 6});
    REQUIRE(world.has<Pos2D>(e));
    REQUIRE(world.has<Vel2D>(e));

    world.remove<Vel2D>(e);
    REQUIRE(world.has<Pos2D>(e));
    REQUIRE_FALSE(world.has<Vel2D>(e));
    // Position value preserved after archetype move
    REQUIRE(world.get<Pos2D>(e).x == 1.0f);
}

TEST_CASE("ECS2 Entity — 1000 create/destroy cycle", "[ecs2][entity][stress]") {
    World2 world;
    std::vector<EntityId> ids;
    ids.reserve(1000);
    for (int i = 0; i < 1000; ++i) {
        EntityId e = world.create();
        world.add<Pos2D>(e, {(float)i, 0});
        ids.push_back(e);
    }
    REQUIRE(world.entity_count() == 1000);

    for (auto e : ids) world.destroy(e);
    REQUIRE(world.entity_count() == 0);
}

TEST_CASE("ECS2 Entity — for_each iterates correct count", "[ecs2][entity]") {
    World2 world;
    for (int i = 0; i < 50; ++i) {
        EntityId e = world.create();
        world.add<Pos2D>(e, {(float)i, (float)i});
        if (i % 2 == 0) world.add<Vel2D>(e, {1, 0});
    }

    int pos_count = 0;
    world.for_each<Pos2D>([&](Pos2D&) { ++pos_count; });
    REQUIRE(pos_count == 50);

    int both_count = 0;
    world.for_each<Pos2D, Vel2D>([&](Pos2D&, Vel2D&) { ++both_count; });
    REQUIRE(both_count == 25);
}

TEST_CASE("ECS2 Entity — for_each with EntityId", "[ecs2][entity]") {
    World2 world;
    EntityId e1 = world.create();
    world.add<Mass>(e1, {10.0f});
    EntityId e2 = world.create();
    world.add<Mass>(e2, {20.0f});

    float total_mass = 0;
    world.for_each<Mass>([&](EntityId, Mass& m) { total_mass += m.kg; });
    REQUIRE(total_mass == 30.0f);
}
