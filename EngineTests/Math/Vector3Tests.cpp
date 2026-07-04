#include <catch2/catch_all.hpp>
#include "ShapeEngine/Math/Vector3.hpp"

using namespace Shape::Math;

TEST_CASE("Vector3 basic structures and operations", "[Math]") {
    Vector3f a(1.0f, 2.0f, 3.0f);
    Vector3f b(4.0f, 5.0f, 6.0f);

    SECTION("Constructors and components") {
        Vector3f zero = Vector3f::Zero();
        REQUIRE(zero.x == 0.0f);
        REQUIRE(zero.y == 0.0f);
        REQUIRE(zero.z == 0.0f);

        Vector3f one(1.0f);
        REQUIRE(one.x == 1.0f);
        REQUIRE(one.y == 1.0f);
        REQUIRE(one.z == 1.0f);

        REQUIRE(a[0] == 1.0f);
        REQUIRE(a[1] == 2.0f);
        REQUIRE(a[2] == 3.0f);
    }

    SECTION("Operators arithmetic") {
        Vector3f sum = a + b;
        REQUIRE(sum.x == 5.0f);
        REQUIRE(sum.y == 7.0f);
        REQUIRE(sum.z == 9.0f);

        Vector3f scaled = a * 2.0f;
        REQUIRE(scaled.x == 2.0f);
        REQUIRE(scaled.y == 4.0f);
        REQUIRE(scaled.z == 6.0f);

        REQUIRE(a != b);
    }

    SECTION("Dot and Cross products") {
        REQUIRE(Dot(a, b) == 32.0f);

        Vector3f x = Vector3f::UnitX();
        Vector3f y = Vector3f::UnitY();
        Vector3f z = Cross(x, y);
        REQUIRE(z == Vector3f::UnitZ());
    }

    SECTION("Distance and Normalizations") {
        Vector3f vec(3.0f, 0.0f, 4.0f);
        REQUIRE(Length(vec) == 5.0f);
        REQUIRE(LengthSquared(vec) == 25.0f);

        Vector3f normalized = Normalize(vec);
        REQUIRE(NearEquals(Length(normalized), 1.0f));

        Vector3f zero = Vector3f::Zero();
        REQUIRE(SafeNormalize(zero) == Vector3f::Zero());
    }

    SECTION("Reflections and Projections") {
        Vector3f incident(1.0f, -1.0f, 0.0f);
        Vector3f normal(0.0f, 1.0f, 0.0f);
        Vector3f reflected = Reflect(incident, normal);
        REQUIRE(reflected.x == 1.0f);
        REQUIRE(reflected.y == 1.0f);
        REQUIRE(reflected.z == 0.0f);

        Vector3f v(2.0f, 3.0f, 0.0f);
        Vector3f target(1.0f, 0.0f, 0.0f);
        Vector3f projected = Project(v, target);
        REQUIRE(projected.x == 2.0f);
        REQUIRE(projected.y == 0.0f);
        REQUIRE(projected.z == 0.0f);

        Vector3f rejected = Reject(v, target);
        REQUIRE(rejected.x == 0.0f);
        REQUIRE(rejected.y == 3.0f);
        REQUIRE(rejected.z == 0.0f);
    }

    SECTION("Lerp and AngleBetween") {
        Vector3f lerped = Vector3f::Lerp(a, b, 0.5f);
        REQUIRE(lerped.x == 2.5f);
        REQUIRE(lerped.y == 3.5f);
        REQUIRE(lerped.z == 4.5f);

        Vector3f x = Vector3f::UnitX();
        Vector3f y = Vector3f::UnitY();
        REQUIRE(NearEquals(AngleBetween(x, y), PI / 2.0f));
    }

    SECTION("Hash specializations") {
        std::hash<Vector3f> hasher;
        REQUIRE(hasher(a) != hasher(b));
    }
}
