#include <catch2/catch_test_macros.hpp>
#include "ECS/World.hpp"
#include "ECS/CommandBuffer.hpp"

using namespace Shape;

struct PositionComponent {
    static constexpr u32 TypeId = 999;
    f32 x;
    f32 y;
};

struct VelocityComponent {
    static constexpr u32 TypeId = 998;
    f32 dx;
    f32 dy;
};

struct TagComponent {
    static constexpr u32 TypeId = 997;
    const char* name;
};

TEST_CASE("ECS Entity Lifetime and Reuse", "[ECS]") {
    Shape::World world;

    Entity e1 = world.CreateEntity();
    Entity e2 = world.CreateEntity();

    REQUIRE(world.IsEntityValid(e1));
    REQUIRE(world.IsEntityValid(e2));
    REQUIRE(GetEntityIndex(e1) == 0);
    REQUIRE(GetEntityIndex(e2) == 1);
    REQUIRE(GetEntityGeneration(e1) == 0);

    world.DestroyEntity(e1);
    REQUIRE_FALSE(world.IsEntityValid(e1));

    // Creating a new entity should recycle index 0 and increment generation
    Entity e3 = world.CreateEntity();
    REQUIRE(world.IsEntityValid(e3));
    REQUIRE(GetEntityIndex(e3) == 0);
    REQUIRE(GetEntityGeneration(e3) == 1);
}

TEST_CASE("ECS Component Storage", "[ECS]") {
    Shape::World world;
    Entity e1 = world.CreateEntity();

    SECTION("Adding and retrieving components") {
        world.AddComponent<PositionComponent>(e1, {10.0f, 20.0f});
        REQUIRE(world.HasComponent<PositionComponent>(e1));
        
        PositionComponent& pos = world.GetComponent<PositionComponent>(e1);
        REQUIRE(pos.x == 10.0f);
        REQUIRE(pos.y == 20.0f);

        pos.x = 50.0f;
        REQUIRE(world.GetComponent<PositionComponent>(e1).x == 50.0f);
    }

    SECTION("Removing components") {
        world.AddComponent<PositionComponent>(e1, {10.0f, 20.0f});
        REQUIRE(world.HasComponent<PositionComponent>(e1));

        world.RemoveComponent<PositionComponent>(e1);
        REQUIRE_FALSE(world.HasComponent<PositionComponent>(e1));
    }

    SECTION("Component cleanup on Entity Destruction") {
        world.AddComponent<PositionComponent>(e1, {10.0f, 20.0f});
        world.AddComponent<VelocityComponent>(e1, {1.0f, 0.0f});

        world.DestroyEntity(e1);
        
        // Components should be removed automatically
        auto& posStore = world.GetComponentStore<PositionComponent>();
        auto& velStore = world.GetComponentStore<VelocityComponent>();
        REQUIRE(posStore.GetSize() == 0);
        REQUIRE(velStore.GetSize() == 0);
    }
}

TEST_CASE("ECS Query System", "[ECS]") {
    Shape::World world;

    Entity e1 = world.CreateEntity(); // Has Position & Velocity
    Entity e2 = world.CreateEntity(); // Has Position only
    Entity e3 = world.CreateEntity(); // Has Velocity & Tag

    world.AddComponent<PositionComponent>(e1, {0.0f, 0.0f});
    world.AddComponent<VelocityComponent>(e1, {1.0f, 1.0f});

    world.AddComponent<PositionComponent>(e2, {5.0f, 5.0f});

    world.AddComponent<VelocityComponent>(e3, {-1.0f, 0.0f});
    world.AddComponent<TagComponent>(e3, {"Player"});

    SECTION("Query for single component") {
        auto entities = world.Query<PositionComponent>();
        REQUIRE(entities.size() == 2);
        REQUIRE(std::find(entities.begin(), entities.end(), e1) != entities.end());
        REQUIRE(std::find(entities.begin(), entities.end(), e2) != entities.end());
    }

    SECTION("Query for multiple components (AND logic)") {
        auto entities = world.Query<PositionComponent, VelocityComponent>();
        REQUIRE(entities.size() == 1);
        REQUIRE(entities[0] == e1);
    }

    SECTION("Query with no matches") {
        auto entities = world.Query<PositionComponent, TagComponent>();
        REQUIRE(entities.empty());
    }
}

TEST_CASE("ECS Command Buffer (Deferred Changes)", "[ECS]") {
    Shape::World world;
    Shape::CommandBuffer cmd(world);

    Entity e1 = world.CreateEntity();

    cmd.AddComponentDeferred<PositionComponent>(e1, {100.0f, 200.0f});
    cmd.RemoveComponentDeferred<PositionComponent>(e1); // Remove immediately after add
    cmd.AddComponentDeferred<VelocityComponent>(e1, {2.0f, 2.0f});

    // Before flushing, no changes should be applied
    REQUIRE_FALSE(world.HasComponent<PositionComponent>(e1));
    REQUIRE_FALSE(world.HasComponent<VelocityComponent>(e1));

    // Flush commands
    cmd.Flush();

    // After flush, changes should be applied
    REQUIRE_FALSE(world.HasComponent<PositionComponent>(e1)); // Was removed in the queue
    REQUIRE(world.HasComponent<VelocityComponent>(e1));
    REQUIRE(world.GetComponent<VelocityComponent>(e1).dx == 2.0f);
}
