#pragma once

#include "Core/Platform.hpp"
#include "Math/Vector3.hpp"

namespace Shape::Math {

struct Matrix4 {
    // 4x4 array in column-major order: m[column][row]
    f32 m[4][4] = {
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f}
    };

    constexpr Matrix4() = default;

    constexpr Matrix4(
        f32 m00, f32 m01, f32 m02, f32 m03,
        f32 m10, f32 m11, f32 m12, f32 m13,
        f32 m20, f32 m21, f32 m22, f32 m23,
        f32 m30, f32 m31, f32 m32, f32 m33
    ) {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
        m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
    }

    // Accessors
    constexpr f32* operator[](usize col) { return m[col]; }
    constexpr const f32* operator[](usize col) const { return m[col]; }

    // Operator overloads
    Matrix4 operator*(const Matrix4& other) const;
    Vector3 MultiplyPoint(const Vector3& point) const;     // Applies translation, rotation, and scaling (W = 1)
    Vector3 MultiplyDirection(const Vector3& dir) const;  // Applies only rotation and scaling (W = 0)

    constexpr bool operator==(const Matrix4& other) const {
        for (int c = 0; c < 4; ++c) {
            for (int r = 0; r < 4; ++r) {
                if (!NearEquals(m[c][r], other.m[c][r])) return false;
            }
        }
        return true;
    }
    constexpr bool operator!=(const Matrix4& other) const { return !(*this == other); }

    // Transform Operations
    Matrix4 Transposed() const;
    void Transpose();

    Matrix4 Inverted() const;
    void Invert();
    f32 Determinant() const;

    // Static transformation helpers
    static constexpr Matrix4 Identity() {
        return Matrix4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    static Matrix4 Translation(const Vector3& translation);
    static Matrix4 Rotation(f32 angleRadians, const Vector3& axis);
    static Matrix4 Scaling(const Vector3& scale);
    
    static Matrix4 Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 nearPlane, f32 farPlane);
    static Matrix4 Perspective(f32 fovRadians, f32 aspect, f32 nearPlane, f32 farPlane);
    static Matrix4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
};

} // namespace Shape::Math
