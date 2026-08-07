#include <catch2/catch_test_macros.hpp>
#include "Spatial/SpatialHash.hpp"
#include "Spatial/BroadPhaseSystem.hpp"
#include "ECS/World.hpp"

TEST_CASE("Spatial Subsystem - SpatialHash and BroadPhaseSystem", "[Spatial][Grid]") {
    SECTION("SpatialHash cell indexing and radius queries") {
        Shape::Spatial::SpatialHash<u32> grid(50.0f);
        grid.Insert(100, Shape::Math::Vector2f(10.0f, 20.0f));
        grid.Insert(101, Shape::Math::Vector2f(30.0f, 40.0f));
        grid.Insert(102, Shape::Math::Vector2f(500.0f, 500.0f));

        auto neighbors = grid.QueryRadius(Shape::Math::Vector2f(15.0f, 25.0f), 50.0f);
        REQUIRE(neighbors.size() == 2);

        grid.Clear();
        auto emptyQuery = grid.QueryRadius(Shape::Math::Vector2f(15.0f, 25.0f), 50.0f);
        REQUIRE(emptyQuery.empty());
    }

    SECTION("BroadPhaseSystem ECS integration") {
        Shape::World world;
        Shape::Spatial::BroadPhaseSystem broadPhase(40.0f);

        Shape::Entity e1 = world.CreateEntity();
        world.AddComponent<Shape::TransformComponent>(e1, Shape::TransformComponent{ .position = { 0.0f, 0.0f } });

        Shape::Entity e2 = world.CreateEntity();
        world.AddComponent<Shape::TransformComponent>(e2, Shape::TransformComponent{ .position = { 15.0f, 15.0f } });

        Shape::Entity e3 = world.CreateEntity();
        world.AddComponent<Shape::TransformComponent>(e3, Shape::TransformComponent{ .position = { 1000.0f, 1000.0f } });

        Shape::UpdateContext ctx;
        broadPhase.Update(world, ctx);

        const auto& hash = broadPhase.GetSpatialHash();
        auto nearby = hash.QueryRadius(Shape::Math::Vector2f(5.0f, 5.0f), 50.0f);
        REQUIRE(nearby.size() == 2);
    }
}
