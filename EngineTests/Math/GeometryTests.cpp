#include <catch2/catch_all.hpp>
#include "ShapeEngine/Math/Geometry/Point.hpp"
#include "ShapeEngine/Math/Geometry/Line.hpp"
#include "ShapeEngine/Math/Geometry/Ray.hpp"
#include "ShapeEngine/Math/Geometry/Segment.hpp"
#include "ShapeEngine/Math/Geometry/Plane.hpp"
#include "ShapeEngine/Math/Geometry/Triangle.hpp"
#include "ShapeEngine/Math/Geometry/Sphere.hpp"
#include "ShapeEngine/Math/Geometry/Capsule.hpp"
#include "ShapeEngine/Math/Geometry/AABB.hpp"

using namespace Shape::Math;

TEST_CASE("Geometry primitives", "[Math][Geometry]") {
    SECTION("Point3 operations") {
        Point3f a(1.0f, 2.0f, 3.0f);
        Point3f b(4.0f, 6.0f, 3.0f);
        Vector3f diff = b - a;
        REQUIRE(NearEquals(diff.x, 3.0f));
        REQUIRE(NearEquals(diff.y, 4.0f));
        REQUIRE(NearEquals(Distance(a, b), 5.0f));
    }

    SECTION("Ray closest point") {
        Ray3f ray(Point3f(0,0,0), Vector3f::UnitX());
        Point3f p(3.0f, 2.0f, 0.0f);
        Point3f closest = ray.ClosestPoint(p);
        REQUIRE(NearEquals(closest.x, 3.0f));
        REQUIRE(NearEquals(closest.y, 0.0f));
    }

    SECTION("Ray rejects negative t") {
        Ray3f ray(Point3f(0,0,0), Vector3f::UnitX());
        Point3f behind(-5.0f, 1.0f, 0.0f);
        Point3f closest = ray.ClosestPoint(behind);
        REQUIRE(NearEquals(closest.x, 0.0f));
    }

    SECTION("Segment closest point and length") {
        Segment3f seg(Point3f(0,0,0), Point3f(10,0,0));
        REQUIRE(NearEquals(seg.Length(), 10.0f));

        Point3f p(5.0f, 3.0f, 0.0f);
        Point3f closest = seg.ClosestPoint(p);
        REQUIRE(NearEquals(closest.x, 5.0f));
        REQUIRE(NearEquals(closest.y, 0.0f));

        // Clamping past end
        Point3f far(20.0f, 0.0f, 0.0f);
        REQUIRE(NearEquals(seg.ClosestPoint(far).x, 10.0f));
    }

    SECTION("Sphere containment and distance") {
        Sphere3f sphere(Point3f(0,0,0), 5.0f);
        REQUIRE(sphere.Contains(Point3f(3,0,0)));
        REQUIRE_FALSE(sphere.Contains(Point3f(6,0,0)));
        REQUIRE(NearEquals(sphere.Distance(Point3f(8.0f,0,0)), 3.0f));
        REQUIRE(NearEquals(sphere.Distance(Point3f(2,0,0)), 0.0f));
    }

    SECTION("AABB containment and closest point") {
        AABB3f box(Point3f(-1,-1,-1), Point3f(1,1,1));
        REQUIRE(box.Contains(Point3f(0,0,0)));
        REQUIRE_FALSE(box.Contains(Point3f(2,0,0)));

        Point3f outside(3.0f, 0.0f, 0.0f);
        Point3f closest = box.ClosestPoint(outside);
        REQUIRE(NearEquals(closest.x, 1.0f));
        REQUIRE(NearEquals(box.Distance(outside), 2.0f));
    }

    SECTION("Triangle normal and centroid") {
        Triangle3f tri(
            Point3f(0,0,0),
            Point3f(1,0,0),
            Point3f(0,1,0)
        );
        Vector3f normal = tri.Normal();
        REQUIRE(NearEquals(normal.z, 1.0f));
        REQUIRE(NearEquals(tri.Area(), 0.5f));
    }
}
