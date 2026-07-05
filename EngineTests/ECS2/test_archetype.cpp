/// @file test_archetype.cpp — ECS2 Archetype ID and ArchetypeStorage tests

#include <catch2/catch_test_macros.hpp>
#include "ECS2/Archetype.hpp"
#include "ECS2/ArchetypeStorage.hpp"
#include "ECS2/ComponentRegistry.hpp"

using namespace Shape::ECS2;

struct CompA { int a = 0; };
struct CompB { float b = 0; };
struct CompC { double c = 0; };

struct ArchReg {
    ArchReg() {
        SHAPE_REGISTER_COMPONENT(CompA);
        SHAPE_REGISTER_COMPONENT(CompB);
        SHAPE_REGISTER_COMPONENT(CompC);
    }
};
static ArchReg _areg;

TEST_CASE("ArchetypeId — identical sets are equal", "[ecs2][archetype]") {
    ArchetypeId a({{component_id<CompA>(), component_id<CompB>()}});
    ArchetypeId b({{component_id<CompB>(), component_id<CompA>()}}); // different order
    REQUIRE(a == b);
}

TEST_CASE("ArchetypeId — contains() works", "[ecs2][archetype]") {
    ArchetypeId id({{component_id<CompA>(), component_id<CompB>()}});
    REQUIRE(id.contains(component_id<CompA>()));
    REQUIRE(id.contains(component_id<CompB>()));
    REQUIRE_FALSE(id.contains(component_id<CompC>()));
}

TEST_CASE("ArchetypeId — with() adds component", "[ecs2][archetype]") {
    ArchetypeId id({{component_id<CompA>()}});
    ArchetypeId extended = id.with(component_id<CompB>());
    REQUIRE(extended.contains(component_id<CompA>()));
    REQUIRE(extended.contains(component_id<CompB>()));
    REQUIRE(extended.size() == 2);
}

TEST_CASE("ArchetypeId — without() removes component", "[ecs2][archetype]") {
    ArchetypeId id({{component_id<CompA>(), component_id<CompB>()}});
    ArchetypeId reduced = id.without(component_id<CompB>());
    REQUIRE(reduced.contains(component_id<CompA>()));
    REQUIRE_FALSE(reduced.contains(component_id<CompB>()));
    REQUIRE(reduced.size() == 1);
}

TEST_CASE("ArchetypeId — contains_all() subset check", "[ecs2][archetype]") {
    ArchetypeId full({{component_id<CompA>(), component_id<CompB>(), component_id<CompC>()}});
    ArchetypeId sub({{component_id<CompA>(), component_id<CompC>()}});
    REQUIRE(full.contains_all(sub));
    REQUIRE_FALSE(sub.contains_all(full));
}

TEST_CASE("ArchetypeStorage — alloc and free rows", "[ecs2][archetype]") {
    ArchetypeId id({{component_id<CompA>()}});
    ArchetypeStorage storage(id);

    EntityId e1{0, 1}, e2{1, 1}, e3{2, 1};
    auto [ci0, r0] = storage.alloc_row(e1);
    auto [ci1, r1] = storage.alloc_row(e2);
    auto [ci2, r2] = storage.alloc_row(e3);

    REQUIRE(storage.entity_count() == 3);

    // Write values
    *storage.get<CompA>(ci0, r0) = {10};
    *storage.get<CompA>(ci1, r1) = {20};
    *storage.get<CompA>(ci2, r2) = {30};

    REQUIRE(storage.get<CompA>(ci0, r0)->a == 10);
    REQUIRE(storage.get<CompA>(ci1, r1)->a == 20);

    // Free middle
    EntityId moved = storage.free_row(ci1, r1);
    REQUIRE(storage.entity_count() == 2);
    // After swap-erase, moved entity (e3) should have moved into slot r1
    (void)moved;
}

TEST_CASE("ArchetypeStorage — chunk capacity non-zero", "[ecs2][archetype]") {
    ArchetypeId id({{component_id<CompA>(), component_id<CompB>()}});
    ArchetypeStorage storage(id);
    REQUIRE(storage.chunk_capacity() > 0);
    // 16KB chunk with CompA(4B) + CompB(4B) + EntityId(8B) should hold many
    REQUIRE(storage.chunk_capacity() >= 100);
}
