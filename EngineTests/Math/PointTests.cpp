#include <catch2/catch_all.hpp>
#include "ShapeEngine/Math/Geometry/Point/PointTraits.hpp"
#include "ShapeEngine/Math/Geometry/Point/PointAlgorithms.hpp"

using namespace Shape::Math;
using namespace Shape::Math::Geometry;

// ─────────────────────────────────────────────────────────────────────────────
//  Section 1: Geometry Core Framework — compile-time checks
// ─────────────────────────────────────────────────────────────────────────────

// These checks run at compile time — if they fail the translation unit fails.
static_assert(GeometryDimension_v<Point2f>  == 2, "Dimension");
static_assert(GeometryDimension_v<Point3f>  == 3, "Dimension");
static_assert(GeometryDimension_v<Point4f>  == 4, "Dimension");

static_assert(Point2f::GeomType == GeometryType::Point2, "GeomType");
static_assert(Point3f::GeomType == GeometryType::Point3, "GeomType");
static_assert(Point4f::GeomType == GeometryType::Point4, "GeomType");

// Concept satisfaction (would cause compile error if failing)
static_assert(PointGeometry<Point2f>, "Point2f must satisfy PointGeometry");
static_assert(PointGeometry<Point3f>, "Point3f must satisfy PointGeometry");
static_assert(PointGeometry<Point4f>, "Point4f must satisfy PointGeometry");

// ─────────────────────────────────────────────────────────────────────────────
//  Section 2: Point2 tests
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Point2: construction and validity", "[Geometry][Point2]") {
    SECTION("Default is origin") {
        Point2f p;
        REQUIRE(p.x == 0.0f); REQUIRE(p.y == 0.0f);
        REQUIRE(p.IsValid());
    }
    SECTION("Named origin") {
        REQUIRE(Point2f::Origin() == Point2f(0, 0));
    }
}

TEST_CASE("Point2: arithmetic type safety", "[Geometry][Point2]") {
    SECTION("Point + Vector = Point") {
        Point2f  p(3.0f, 4.0f);
        Vector2f v(1.0f, 2.0f);
        Point2f  result = p + v;
        REQUIRE(result.x == Catch::Approx(4.0f));
        REQUIRE(result.y == Catch::Approx(6.0f));
    }
    SECTION("Point - Vector = Point") {
        Point2f p(5.0f, 5.0f);
        Vector2f v(2.0f, 3.0f);
        Point2f r = p - v;
        REQUIRE(r.x == Catch::Approx(3.0f));
        REQUIRE(r.y == Catch::Approx(2.0f));
    }
    SECTION("Point - Point = Vector") {
        Point2f a(1.0f, 2.0f), b(4.0f, 6.0f);
        Vector2f d = b - a;
        REQUIRE(d.x == Catch::Approx(3.0f));
        REQUIRE(d.y == Catch::Approx(4.0f));
    }
}

TEST_CASE("Point2: distance and midpoint", "[Geometry][Point2]") {
    SECTION("Distance") {
        Point2f a(0, 0), b(3.0f, 4.0f);
        REQUIRE(Distance(a, b) == Catch::Approx(5.0f));
    }
    SECTION("DistanceSquared") {
        Point2f a(0, 0), b(3.0f, 4.0f);
        REQUIRE(DistanceSquared(a, b) == Catch::Approx(25.0f));
    }
    SECTION("Midpoint") {
        Point2f a(0, 0), b(4.0f, 2.0f);
        Point2f m = Midpoint(a, b);
        REQUIRE(m.x == Catch::Approx(2.0f));
        REQUIRE(m.y == Catch::Approx(1.0f));
    }
}

TEST_CASE("Point2: NearEquals", "[Geometry][Point2]") {
    Point2f a(1.0f, 2.0f);
    Point2f b(1.0f + 1e-7f, 2.0f - 1e-7f);
    REQUIRE(a.NearEquals(b));
    Point2f c(1.1f, 2.0f);
    REQUIRE_FALSE(a.NearEquals(c));
}

// ─────────────────────────────────────────────────────────────────────────────
//  Section 3: Point3 tests
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Point3: construction and validity", "[Geometry][Point3]") {
    SECTION("Default is origin") {
        Point3f p;
        REQUIRE(p.x == 0.0f); REQUIRE(p.y == 0.0f); REQUIRE(p.z == 0.0f);
        REQUIRE(p.IsValid());
    }
    SECTION("Named origin") {
        REQUIRE(Point3f::Origin() == Point3f(0, 0, 0));
    }
}

TEST_CASE("Point3: arithmetic type safety", "[Geometry][Point3]") {
    SECTION("Point + Vector = Point") {
        Point3f  p(1, 2, 3);
        Vector3f v(10, 20, 30);
        Point3f  r = p + v;
        REQUIRE(r.x == Catch::Approx(11.0f));
        REQUIRE(r.y == Catch::Approx(22.0f));
        REQUIRE(r.z == Catch::Approx(33.0f));
    }
    SECTION("Point - Point = Vector") {
        Point3f a(0, 0, 0), b(1.0f, 2.0f, 3.0f);
        Vector3f d = b - a;
        REQUIRE(d.x == Catch::Approx(1.0f));
        REQUIRE(d.y == Catch::Approx(2.0f));
        REQUIRE(d.z == Catch::Approx(3.0f));
    }
}

TEST_CASE("Point3: distance and midpoint", "[Geometry][Point3]") {
    SECTION("Distance 3D Pythagorean") {
        Point3f a(0, 0, 0), b(1.0f, 2.0f, 2.0f);
        REQUIRE(Distance(a, b) == Catch::Approx(3.0f));
    }
    SECTION("Lerp at t=0.5") {
        Point3f a(0, 0, 0), b(10.0f, 0, 0);
        Point3f m = LerpPoint(a, b, 0.5f);
        REQUIRE(m.x == Catch::Approx(5.0f));
    }
    SECTION("Centroid of 4 points") {
        Point3f pts[4] = { {0,0,0}, {4,0,0}, {4,4,0}, {0,4,0} };
        Point3f cen = Centroid(pts, 4);
        REQUIRE(cen.x == Catch::Approx(2.0f));
        REQUIRE(cen.y == Catch::Approx(2.0f));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Section 4: Point4 / homogeneous tests
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Point4: homogeneous operations", "[Geometry][Point4]") {
    SECTION("Default w=1") {
        Point4f p;
        REQUIRE(p.w == 1.0f);
    }
    SECTION("Perspective divide") {
        Point4f p(4.0f, 6.0f, 8.0f, 2.0f);
        Point3f r = p.PerspectiveDivide();
        REQUIRE(r.x == Catch::Approx(2.0f));
        REQUIRE(r.y == Catch::Approx(3.0f));
        REQUIRE(r.z == Catch::Approx(4.0f));
    }
    SECTION("Infinity detection") {
        Point4f inf(1.0f, 0.0f, 0.0f, 0.0f);
        REQUIRE(inf.IsAtInfinity());
        REQUIRE_FALSE(Point4f::Origin().IsAtInfinity());
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Section 5: PointAlgorithms tests
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("PointAlgorithms: bounding box", "[Geometry][PointAlgorithms]") {
    Point3f pts[5] = {
        {-3, 1, 2}, {5, -2, 0}, {0, 0, 7}, {4, 3, -1}, {-1, 6, 3}
    };
    auto bbox = BoundingBox(pts, 5);
    REQUIRE(bbox.minPt.x == Catch::Approx(-3.0f));
    REQUIRE(bbox.maxPt.x == Catch::Approx(5.0f));
    REQUIRE(bbox.minPt.y == Catch::Approx(-2.0f));
    REQUIRE(bbox.maxPt.y == Catch::Approx(6.0f));
}

TEST_CASE("PointAlgorithms: Ritter sphere", "[Geometry][PointAlgorithms]") {
    Point3f pts[4] = { {-5,0,0}, {5,0,0}, {0,5,0}, {0,-5,0} };
    auto s = RitterSphere(pts, 4);
    // All points must lie inside or on surface
    for (const auto& p : pts) {
        float d = Distance(s.center, p);
        REQUIRE(d <= s.radius + 1e-4f);
    }
}

TEST_CASE("PointAlgorithms: closest pair 2D", "[Geometry][PointAlgorithms]") {
    Point2f pts[5] = { {0,0}, {10,10}, {10.1f,10.1f}, {5,5}, {-3,-3} };
    auto result = ClosestPair2D(pts, 5);
    // Closest pair should be indices 1 and 2
    bool found = (result.indexA == 1 && result.indexB == 2) ||
                 (result.indexA == 2 && result.indexB == 1);
    REQUIRE(found);
}

TEST_CASE("PointAlgorithms: circumcircle", "[Geometry][PointAlgorithms]") {
    SECTION("Right triangle") {
        // For a right triangle inscribed in a circle, hypotenuse = diameter
        Point2f a(0,0), b(4,0), c(0,3);
        auto [center, radius, valid] = Circumcircle(a, b, c);
        REQUIRE(valid);
        // All three points at radius from center
        REQUIRE(Distance(center, a) == Catch::Approx(radius).epsilon(1e-4f));
        REQUIRE(Distance(center, b) == Catch::Approx(radius).epsilon(1e-4f));
        REQUIRE(Distance(center, c) == Catch::Approx(radius).epsilon(1e-4f));
    }
    SECTION("Collinear points return invalid") {
        Point2f a(0,0), b(1,0), c(2,0);
        auto [center, radius, valid] = Circumcircle(a, b, c);
        REQUIRE_FALSE(valid);
    }
}
