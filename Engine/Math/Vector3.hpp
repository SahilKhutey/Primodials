#pragma once

#include "Math/MathUtils.hpp"
#include "ShapeEngine/Math/Vector2.hpp"
#include "Core/Platform.hpp"

namespace Shape::Math {

struct Vector3 {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;

    constexpr Vector3() = default;
    constexpr Vector3(f32 inX, f32 inY, f32 inZ) : x(inX), y(inY), z(inZ) {}
    constexpr Vector3(const Vector2f& vec2, f32 inZ) : x(vec2.x), y(vec2.y), z(inZ) {}
    explicit constexpr Vector3(f32 scalar) : x(scalar), y(scalar), z(scalar) {}

    // Operator overloads
    constexpr Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
    constexpr Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    constexpr Vector3 operator*(const Vector3& other) const { return Vector3(x * other.x, y * other.y, z * other.z); }
    constexpr Vector3 operator/(const Vector3& other) const { return Vector3(x / other.x, y / other.y, z / other.z); }

    constexpr Vector3 operator*(f32 scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    constexpr Vector3 operator/(f32 scalar) const {
        f32 factor = 1.0f / scalar;
        return Vector3(x * factor, y * factor, z * factor);
    }

    constexpr Vector3& operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
    constexpr Vector3& operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    constexpr Vector3& operator*=(const Vector3& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
    constexpr Vector3& operator/=(const Vector3& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }

    constexpr Vector3& operator*=(f32 scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    constexpr Vector3& operator/=(f32 scalar) {
        f32 factor = 1.0f / scalar;
        x *= factor; y *= factor; z *= factor;
        return *this;
    }

    constexpr Vector3 operator-() const { return Vector3(-x, -y, -z); }

    constexpr bool operator==(const Vector3& other) const {
        return NearEquals(x, other.x) && NearEquals(y, other.y) && NearEquals(z, other.z);
    }
    constexpr bool operator!=(const Vector3& other) const { return !(*this == other); }

    // Vector operations
    constexpr f32 LengthSq() const { return x * x + y * y + z * z; }
    f32 Length() const { return Sqrt(LengthSq()); }

    SHAPE_INLINE Vector3 Normalized() const {
        f32 len = Length();
        if (len > EPSILON) {
            return *this / len;
        }
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    SHAPE_INLINE void Normalize() {
        f32 len = Length();
        if (len > EPSILON) {
            *this /= len;
        } else {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }
    }

    constexpr f32 Dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }

    constexpr Vector3 Cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    constexpr f32 DistanceSq(const Vector3& other) const {
        f32 dx = x - other.x;
        f32 dy = y - other.y;
        f32 dz = z - other.z;
        return dx * dx + dy * dy + dz * dz;
    }
    f32 Distance(const Vector3& other) const { return Sqrt(DistanceSq(other)); }

    constexpr Vector2f ToVector2() const { return Vector2f(x, y); }

    static const Vector3 Zero;
    static const Vector3 One;
    static const Vector3 Right;
    static const Vector3 Left;
    static const Vector3 Up;
    static const Vector3 Down;
    static const Vector3 Forward;
    static const Vector3 Back;
};

// Inline definitions of static constants
inline constexpr Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);
inline constexpr Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
inline constexpr Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);
inline constexpr Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);
inline constexpr Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
inline constexpr Vector3 Vector3::Down(0.0f, -1.0f, 0.0f);
inline constexpr Vector3 Vector3::Forward(0.0f, 0.0f, 1.0f);
inline constexpr Vector3 Vector3::Back(0.0f, 0.0f, -1.0f);

// Left-hand scalar multiplication
inline constexpr Vector3 operator*(f32 scalar, const Vector3& vec) {
    return vec * scalar;
}

} // namespace Shape::Math
