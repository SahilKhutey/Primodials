#include <catch2/catch_all.hpp>
#include "ShapeEngine/Math/Matrix2.hpp"

using namespace Shape::Math;
using namespace Shape::Math::Literals;

TEST_CASE("Matrix2 structures and properties", "[Math]") {
    Matrix2f identity = Matrix2f::Identity();
    Matrix2f zero = Matrix2f::Zero();

    SECTION("Identity and Zero matrices traits") {
        REQUIRE(identity.m00 == 1.0f);
        REQUIRE(identity.m11 == 1.0f);
        REQUIRE(identity.m01 == 0.0f);

        REQUIRE(IsIdentity(identity));
        REQUIRE_FALSE(IsIdentity(zero));
        REQUIRE(IsSymmetric(identity));
    }

    SECTION("Scale matrix generation") {
        Matrix2f s = Matrix2f::Scale(2.0f, 3.0f);
        REQUIRE(s.m00 == 2.0f);
        REQUIRE(s.m11 == 3.0f);

        Vector2f v(1.0f, 1.0f);
        Vector2f scaled = s * v;
        REQUIRE(scaled.x == 2.0f);
        REQUIRE(scaled.y == 3.0f);
    }

    SECTION("Rotation matrix generation") {
        Matrix2f rot = Matrix2f::Rotation(90.0_deg);
        Vector2f v(1.0f, 0.0f);
        Vector2f rotated = rot * v;

        REQUIRE(NearEquals(rotated.x, 0.0f));
        REQUIRE(NearEquals(rotated.y, 1.0f));

        REQUIRE(IsOrthogonal(rot));
    }

    SECTION("Transpose and Determinant operations") {
        Matrix2f m(1.0f, 2.0f, 3.0f, 4.0f);
        Matrix2f t = Transpose(m);

        REQUIRE(t.m01 == 3.0f);
        REQUIRE(t.m10 == 2.0f);

        REQUIRE(Determinant(m) == -2.0f);
    }

    SECTION("Inversion operations") {
        Matrix2f m(4.0f, 7.0f, 2.0f, 6.0f);
        auto optInv = SafeInverse(m);
        REQUIRE(optInv.has_value());

        Matrix2f inv = optInv.value();
        Matrix2f prod = m * inv;
        REQUIRE(IsIdentity(prod));

        Matrix2f uninvertible(1.0f, 2.0f, 2.0f, 4.0f);
        REQUIRE_FALSE(SafeInverse(uninvertible).has_value());
    }

    SECTION("Decomposition parameters") {
        Matrix2f s = Matrix2f::Scale(3.0f, 4.0f);
        Vector2f scale = ExtractScale(s);
        REQUIRE(NearEquals(scale.x, 3.0f));
        REQUIRE(NearEquals(scale.y, 4.0f));

        REQUIRE(Trace(s) == 7.0f);
    }
}
