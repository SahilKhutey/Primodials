#include <catch2/catch_test_macros.hpp>
#include "ShapeEngine/Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Matrix4.hpp"

using namespace Shape::Math;

TEST_CASE("Vector2f operations", "[Math]") {
    Vector2f v1(3.0f, 4.0f);
    Vector2f v2(1.0f, 2.0f);

    REQUIRE(v1 + v2 == Vector2f(4.0f, 6.0f));
    REQUIRE(v1 - v2 == Vector2f(2.0f, 2.0f));
    REQUIRE(v1 * 2.0f == Vector2f(6.0f, 8.0f));
    REQUIRE(v1 / 2.0f == Vector2f(1.5f, 2.0f));

    REQUIRE(v1.LengthSquared() == 25.0f);
    REQUIRE(NearEquals(v1.Length(), 5.0f));

    Vector2f norm = v1.Normalized();
    REQUIRE(NearEquals(norm.Length(), 1.0f));
    REQUIRE(NearEquals(norm.x, 3.0f / 5.0f));
    REQUIRE(NearEquals(norm.y, 4.0f / 5.0f));

    REQUIRE(v1.Dot(v2) == 11.0f);
    REQUIRE(v1.Cross(v2) == 2.0f); // 3*2 - 4*1 = 6 - 4 = 2
}

TEST_CASE("Vector3 operations", "[Math]") {
    Vector3 v1(1.0f, 0.0f, 0.0f);
    Vector3 v2(0.0f, 1.0f, 0.0f);

    REQUIRE(v1.Dot(v2) == 0.0f);
    
    // Cross product of Right and Up is Forward (in right-handed system, or depending on sign)
    // Right (1,0,0) Cross Up (0,1,0) = (0, 0, 1)
    REQUIRE(v1.Cross(v2) == Vector3(0.0f, 0.0f, 1.0f));
    
    Vector3 v3(3.0f, 4.0f, 12.0f);
    REQUIRE(v3.LengthSq() == 169.0f);
    REQUIRE(NearEquals(v3.Length(), 13.0f));
}

TEST_CASE("Matrix4 operations", "[Math]") {
    Matrix4 identity = Matrix4::Identity();
    
    SECTION("Identity checking") {
        for (int c = 0; c < 4; ++c) {
            for (int r = 0; r < 4; ++r) {
                if (c == r) {
                    REQUIRE(NearEquals(identity.m[c][r], 1.0f));
                } else {
                    REQUIRE(NearEquals(identity.m[c][r], 0.0f));
                }
            }
        }
    }

    SECTION("Translation matrix") {
        Matrix4 trans = Matrix4::Translation(Vector3(10.0f, -5.0f, 3.0f));
        Vector3 point(1.0f, 2.0f, 3.0f);
        Vector3 result = trans.MultiplyPoint(point);
        REQUIRE(result == Vector3(11.0f, -3.0f, 6.0f));

        Vector3 dir(1.0f, 2.0f, 3.0f);
        Vector3 resultDir = trans.MultiplyDirection(dir);
        REQUIRE(resultDir == dir); // Translation does not affect direction vectors
    }

    SECTION("Scaling matrix") {
        Matrix4 scale = Matrix4::Scaling(Vector3(2.0f, 3.0f, 0.5f));
        Vector3 point(1.0f, 2.0f, 4.0f);
        REQUIRE(scale.MultiplyPoint(point) == Vector3(2.0f, 6.0f, 2.0f));
    }

    SECTION("Matrix multiplication") {
        Matrix4 m1 = Matrix4::Translation(Vector3(5.0f, 0.0f, 0.0f));
        Matrix4 m2 = Matrix4::Scaling(Vector3(2.0f, 2.0f, 2.0f));
        
        // Compound transformation: Scale then Translate
        Matrix4 m3 = m1 * m2;
        Vector3 p(1.0f, 1.0f, 1.0f);
        // Scaling gives (2, 2, 2), then Translation gives (7, 2, 2)
        REQUIRE(m3.MultiplyPoint(p) == Vector3(7.0f, 2.0f, 2.0f));
    }

    SECTION("Transposition") {
        Matrix4 mat(
            1.0f, 2.0f, 3.0f, 4.0f,
            5.0f, 6.0f, 7.0f, 8.0f,
            9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        );
        Matrix4 transp = mat.Transposed();
        for (int c = 0; c < 4; ++c) {
            for (int r = 0; r < 4; ++r) {
                REQUIRE(transp.m[c][r] == mat.m[r][c]);
            }
        }
    }

    SECTION("Determinant and Inversion") {
        // Create an invertible matrix
        Matrix4 mat = Matrix4::Translation(Vector3(2.0f, 3.0f, 4.0f)) * Matrix4::Scaling(Vector3(2.0f, 2.0f, 2.0f));
        f32 det = mat.Determinant();
        REQUIRE(!NearEquals(det, 0.0f));

        Matrix4 inv = mat.Inverted();
        Matrix4 prod1 = mat * inv;
        Matrix4 prod2 = inv * mat;

        REQUIRE(prod1 == Matrix4::Identity());
        REQUIRE(prod2 == Matrix4::Identity());
    }
}
