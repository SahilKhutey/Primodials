#include <catch2/catch_all.hpp>
#include "ShapeEngine/Math/Scalar.hpp"
#include "Math/MathUtils.hpp"

using namespace Shape::Math;

TEST_CASE("Scalar Wrapper basic traits", "[Math]") {
    Scalar<float> val(5.0f);
    REQUIRE(val.Value() == 5.0f);
    REQUIRE(static_cast<float>(val) == 5.0f);

    REQUIRE(Scalar<float>::Min() < 0.0f);
    REQUIRE(Scalar<float>::Max() > 0.0f);
    REQUIRE(Scalar<float>::Epsilon() > 0.0f);
}

TEST_CASE("Common scalar algorithms", "[Math]") {
    SECTION("Clamp functions") {
        REQUIRE(Clamp(15, 0, 10) == 10);
        REQUIRE(Clamp(5, 0, 10) == 5);
        REQUIRE(Clamp(-5, 0, 10) == 0);
    }

    SECTION("Min functions") {
        REQUIRE(Min(3, 5) == 3);
        REQUIRE(Min(5, 3) == 3);
    }

    SECTION("Max functions") {
        REQUIRE(Max(3, 5) == 5);
        REQUIRE(Max(5, 3) == 5);
    }

    SECTION("Abs functions") {
        REQUIRE(Abs(-10) == 10);
        REQUIRE(Abs(10) == 10);
    }

    SECTION("Square functions") {
        REQUIRE(Square(6) == 36);
    }

    SECTION("Cube functions") {
        REQUIRE(Cube(3) == 27);
    }

    SECTION("NearlyEqual checking") {
        REQUIRE(NearEquals(1.0f, 1.0000001f));
        REQUIRE_FALSE(NearEquals(1.0f, 1.1f));
    }

    SECTION("Trigonometric and float helpers") {
        REQUIRE(NearEquals(Sqrt(16.0f), 4.0f));
        REQUIRE(NearEquals(Sin(0.0f), 0.0f));
        REQUIRE(NearEquals(Cos(0.0f), 1.0f));
    }
}
