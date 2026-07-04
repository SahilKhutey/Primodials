#include <catch2/catch_test_macros.hpp>
#include "Simulation/SimulationClock.hpp"
#include "Math/PolygonGeometry.hpp"
#include "Physics/SpatialHashGrid.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "ECS/TransformComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "Physics/ColliderComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include "ECS/World.hpp"

using namespace Shape;
using namespace Shape::Math;

TEST_CASE("Simulation Clock Determinism", "[Physics]") {
    SimulationClock clock(60); // 60Hz tick = 16.6667ms (16666us)
    u32 tickCount = 0;

    auto callback = [&tickCount]() {
        tickCount++;
    };

    SECTION("Normal ticking") {
        // Advancing clock by 50ms (50000us)
        // 50000 / 16666 = 3 ticks with 16666 * 3 = 49998us processed, leaving 2us in accumulator
        clock.Update(50000, callback);
        REQUIRE(tickCount == 3);
        REQUIRE(clock.GetTotalTicks() == 3);
    }

    SECTION("Speed multiplier") {
        clock.SetSpeedMultiplier(2.0f);
        // Advancing by 10ms with 2x speed -> processed as 20ms (20000us)
        // 20000 / 16666 = 1 tick
        clock.Update(10000, callback);
        REQUIRE(tickCount == 1);
    }

    SECTION("Pause check") {
        clock.SetPaused(true);
        clock.Update(100000, callback);
        REQUIRE(tickCount == 0);
    }
}

TEST_CASE("Polygon Geometry Calculations", "[Math]") {
    SECTION("Regular Triangle") {
        // Generating regular 3-gon (Triangle) with radius 10
        std::vector<Vector2f> verts = PolygonGeometry::CreateRegularPolygon(3, 10.0f);
        REQUIRE(verts.size() == 3);

        f32 area = PolygonGeometry::ComputeArea(verts.data(), verts.size());
        f32 perimeter = PolygonGeometry::ComputePerimeter(verts.data(), verts.size());
        Vector2f centroid = PolygonGeometry::ComputeCentroid(verts.data(), verts.size());

        // Exact area of equilateral triangle in circle of radius R: (3 * sqrt(3) / 4) * R^2
        // For R = 10 -> (3 * 1.73205 / 4) * 100 = 129.90
        REQUIRE(NearEquals(area, 129.9038f, 1e-3f));
        REQUIRE(NearEquals(centroid.x, 0.0f));
        REQUIRE(NearEquals(centroid.y, 0.0f));
        REQUIRE(perimeter > 0.0f);
    }

    SECTION("Regular Square") {
        // Regular 4-gon (Square) with radius 10
        std::vector<Vector2f> verts = PolygonGeometry::CreateRegularPolygon(4, 10.0f);
        REQUIRE(verts.size() == 4);

        f32 area = PolygonGeometry::ComputeArea(verts.data(), verts.size());
        // For R = 10 -> Side = sqrt(200) = 14.14 -> Area = Side^2 = 200
        REQUIRE(NearEquals(area, 200.0f, 1e-3f));

        Vector2f centroid = PolygonGeometry::ComputeCentroid(verts.data(), verts.size());
        REQUIRE(NearEquals(centroid.x, 0.0f));
        REQUIRE(NearEquals(centroid.y, 0.0f));
    }
}

TEST_CASE("Spatial Hash Grid Indexing", "[Physics]") {
    SpatialHashGrid grid(10.0f); // 10x10 cell size

    Entity e1 = 1;
    Entity e2 = 2;

    // e1 at (5, 5), size 2 -> occupies cell (0, 0)
    grid.Insert(e1, Vector2f(5.0f, 5.0f), 2.0f);

    // e2 at (15, 15), size 2 -> occupies cell (1, 1)
    grid.Insert(e2, Vector2f(15.0f, 15.0f), 2.0f);

    SECTION("Query overlaps") {
        std::vector<Entity> results;
        grid.Query(Vector2f(4.0f, 4.0f), 1.0f, results);
        REQUIRE(results.size() == 1);
        REQUIRE(results[0] == e1);
    }

    SECTION("Boundary crossing entity") {
        Entity e3 = 3;
        // e3 at (9, 9), size 3 -> overlaps cell bounds, occupying (0,0), (1,0), (0,1), (1,1)
        grid.Insert(e3, Vector2f(9.0f, 9.0f), 3.0f);

        std::vector<Entity> results;
        grid.Query(Vector2f(11.0f, 11.0f), 1.0f, results);
        // Should find e3 (crosses boundary) and e2 (is in (1,1))
        REQUIRE(results.size() == 2);
        REQUIRE(std::find(results.begin(), results.end(), e3) != results.end());
        REQUIRE(std::find(results.begin(), results.end(), e2) != results.end());
    }
}

TEST_CASE("Separating Axis Theorem (SAT) Collisions", "[Physics]") {
    SECTION("Circle vs Circle") {
        CollisionManifold manifold = PhysicsSystem::CheckCircleCollision(
            1, Vector2f(0.0f, 0.0f), 5.0f,
            2, Vector2f(8.0f, 0.0f), 5.0f
        );
        REQUIRE(manifold.colliding);
        REQUIRE(NearEquals(manifold.penetration, 2.0f));
        REQUIRE(manifold.normal == Vector2f(1.0f, 0.0f));

        CollisionManifold manifoldFar = PhysicsSystem::CheckCircleCollision(
            1, Vector2f(0.0f, 0.0f), 5.0f,
            2, Vector2f(11.0f, 0.0f), 5.0f
        );
        REQUIRE_FALSE(manifoldFar.colliding);
    }

    SECTION("Polygon vs Polygon (Colliding Squares)") {
        // Explicitly define axis-aligned square vertices to guarantee 2.0f overlap along x-axis
        std::vector<Vector2f> vertsA = {
            {-5.0f, -5.0f},
            {5.0f, -5.0f},
            {5.0f, 5.0f},
            {-5.0f, 5.0f}
        };
        std::vector<Vector2f> vertsB = {
            {3.0f, -5.0f},
            {13.0f, -5.0f},
            {13.0f, 5.0f},
            {3.0f, 5.0f}
        };

        CollisionManifold manifold = PhysicsSystem::CheckPolygonCollision(
            1, Vector2f(0.0f, 0.0f), vertsA.data(), vertsA.size(),
            2, Vector2f(8.0f, 0.0f), vertsB.data(), vertsB.size()
        );

        REQUIRE(manifold.colliding);
        REQUIRE(NearEquals(manifold.penetration, 2.0f, 0.05f));
        REQUIRE(NearEquals(manifold.normal.x, 1.0f, 0.05f));
        REQUIRE(NearEquals(manifold.normal.y, 0.0f, 0.05f));
    }
}

TEST_CASE("Physics Collision Resolution & Kinetics", "[Physics]") {
    Shape::World world;
    PhysicsSystem physics(10.0f);

    Entity e1 = world.CreateEntity();
    Entity e2 = world.CreateEntity();

    // Register components in ECS registry before retrieval
    world.AddComponent<TransformComponent>(e1, TransformComponent());
    world.AddComponent<RigidBodyComponent>(e1, RigidBodyComponent());
    world.AddComponent<ColliderComponent>(e1, ColliderComponent());

    world.AddComponent<TransformComponent>(e2, TransformComponent());
    world.AddComponent<RigidBodyComponent>(e2, RigidBodyComponent());
    world.AddComponent<ColliderComponent>(e2, ColliderComponent());

    auto& trans1 = world.GetComponent<TransformComponent>(e1);
    auto& body1 = world.GetComponent<RigidBodyComponent>(e1);
    auto& trans2 = world.GetComponent<TransformComponent>(e2);
    auto& body2 = world.GetComponent<RigidBodyComponent>(e2);

    auto& col1 = world.GetComponent<ColliderComponent>(e1);
    auto& col2 = world.GetComponent<ColliderComponent>(e2);

    trans1.position = Vector2f(0.0f, 0.0f);
    body1.velocity = Vector2f(10.0f, 0.0f); // Headed right
    body1.SetMass(1.0f);
    col1.type = ColliderType::Circle;
    col1.radius = 5.0f;
    col1.restitution = 1.0f; // Perfect elastic bounce

    trans2.position = Vector2f(8.0f, 0.0f); // Colliding overlap
    body2.velocity = Vector2f(-10.0f, 0.0f); // Headed left
    body2.SetMass(1.0f);
    col2.type = ColliderType::Circle;
    col2.radius = 5.0f;
    col2.restitution = 1.0f;

    // Run one update tick (dt = 0.016s)
    physics.Update(world, 0.016f);

    // After updating, they should push apart and bounce, swapping velocities
    // e1 should now head left (negative velocity)
    // e2 should now head right (positive velocity)
    REQUIRE(body1.velocity.x < 0.0f);
    REQUIRE(body2.velocity.x > 0.0f);
}
