/// @file test_command_buffer.cpp — ECS2 CommandBuffer2 deferred change tests

#include <catch2/catch_test_macros.hpp>
#include "ECS2/World2.hpp"
#include "ECS2/CommandBuffer2.hpp"
#include "ECS2/ComponentRegistry.hpp"

using namespace Shape::ECS2;

struct CHealth  { int hp = 100; };
struct CDead    { bool dead = true; };
struct CSpawner { int count = 0; };

struct CmdReg {
    CmdReg() {
        SHAPE_REGISTER_COMPONENT(CHealth);
        SHAPE_REGISTER_COMPONENT(CDead);
        SHAPE_REGISTER_COMPONENT(CSpawner);
    }
};
static CmdReg _creg;

TEST_CASE("CommandBuffer2 — deferred destroy", "[ecs2][cmdbuf]") {
    World2 world;
    CommandBuffer2 cmd;

    EntityId e1 = world.create();
    EntityId e2 = world.create();
    world.add<CHealth>(e1, {50});
    world.add<CHealth>(e2, {100});

    // During iteration, queue destroys
    world.for_each<CHealth>([&](EntityId e, CHealth& h) {
        if (h.hp <= 50) cmd.destroy(e);
    });

    REQUIRE(world.entity_count() == 2); // Not yet applied
    cmd.flush(world);
    REQUIRE(world.entity_count() == 1); // e1 destroyed
    REQUIRE_FALSE(world.valid(e1));
    REQUIRE(world.valid(e2));
}

TEST_CASE("CommandBuffer2 — deferred add component", "[ecs2][cmdbuf]") {
    World2 world;
    CommandBuffer2 cmd;

    EntityId e = world.create();
    world.add<CHealth>(e, {0});

    world.for_each<CHealth>([&](EntityId eid, CHealth& h) {
        if (h.hp <= 0) cmd.add<CDead>(eid);
    });

    REQUIRE_FALSE(world.has<CDead>(e));
    cmd.flush(world);
    REQUIRE(world.has<CDead>(e));
}

TEST_CASE("CommandBuffer2 — deferred remove component", "[ecs2][cmdbuf]") {
    World2 world;
    CommandBuffer2 cmd;

    EntityId e = world.create();
    world.add<CHealth>(e, {100});
    world.add<CDead>(e);

    world.for_each<CDead>([&](EntityId eid, CDead&) {
        cmd.remove<CDead>(eid);
    });

    REQUIRE(world.has<CDead>(e));
    cmd.flush(world);
    REQUIRE_FALSE(world.has<CDead>(e));
    REQUIRE(world.has<CHealth>(e)); // Still has health
}

TEST_CASE("CommandBuffer2 — clear discards commands", "[ecs2][cmdbuf]") {
    World2 world;
    CommandBuffer2 cmd;

    EntityId e = world.create();
    cmd.destroy(e);
    cmd.clear();
    cmd.flush(world);

    REQUIRE(world.valid(e)); // Not destroyed — command was cleared
}
