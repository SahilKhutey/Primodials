#pragma once

#include "Core/Platform.hpp"
#include <cmath>
#include <ostream>

namespace Shape::Math {

// ─────────────────────────────────────────────────────────────────────────────
//  Vec2<T> — 2-component vector
// ─────────────────────────────────────────────────────────────────────────────
template<typename T>
struct Vec2 {
    T x{}, y{};

    // Construction
    constexpr Vec2() = default;
    constexpr Vec2(T x_, T y_) : x(x_), y(y_) {}
    explicit constexpr Vec2(T scalar) : x(scalar), y(scalar) {}

    // Named presets
    static constexpr Vec2 Zero()  { return { T(0), T(0) }; }
    static constexpr Vec2 One()   { return { T(1), T(1) }; }
    static constexpr Vec2 UnitX() { return { T(1), T(0) }; }
    static constexpr Vec2 UnitY() { return { T(0), T(1) }; }

    // Arithmetic — component-wise
    constexpr Vec2 operator+(const Vec2& o) const { return { x + o.x, y + o.y }; }
    constexpr Vec2 operator-(const Vec2& o) const { return { x - o.x, y - o.y }; }
    constexpr Vec2 operator*(const Vec2& o) const { return { x * o.x, y * o.y }; }
    constexpr Vec2 operator/(const Vec2& o) const { return { x / o.x, y / o.y }; }

    constexpr Vec2 operator+(T s) const { return { x + s, y + s }; }
    constexpr Vec2 operator-(T s) const { return { x - s, y - s }; }
    constexpr Vec2 operator*(T s) const { return { x * s, y * s }; }
    constexpr Vec2 operator/(T s) const { return { x / s, y / s }; }

    constexpr Vec2 operator-() const { return { -x, -y }; }

    Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }
    Vec2& operator*=(const Vec2& o) { x *= o.x; y *= o.y; return *this; }
    Vec2& operator/=(const Vec2& o) { x /= o.x; y /= o.y; return *this; }
    Vec2& operator*=(T s)           { x *= s;   y *= s;   return *this; }
    Vec2& operator/=(T s)           { x /= s;   y /= s;   return *this; }

    // Comparison
    constexpr bool operator==(const Vec2& o) const { return x == o.x && y == o.y; }
    constexpr bool operator!=(const Vec2& o) const { return !(*this == o); }

    // Indexing
    T&       operator[](int i)       { return (&x)[i]; }
    const T& operator[](int i) const { return (&x)[i]; }

    // Geometry
    constexpr T   Dot(const Vec2& o) const { return x * o.x + y * o.y; }
    constexpr T   Cross(const Vec2& o) const { return x * o.y - y * o.x; }
    constexpr T   LengthSquared() const { return x * x + y * y; }
    T             Length() const        { return static_cast<T>(std::sqrt(static_cast<f64>(LengthSquared()))); }

    Vec2 Normalized() const {
        T len = Length();
        return (len > T(1e-10)) ? Vec2{ x / len, y / len } : Zero();
    }

    void Normalize() { *this = Normalized(); }

    constexpr Vec2 Perpendicular() const { return { -y, x }; }

    T DistanceTo(const Vec2& o) const { return (*this - o).Length(); }
    T DistanceSquaredTo(const Vec2& o) const { return (*this - o).LengthSquared(); }

    Vec2 Lerp(const Vec2& to, f32 t) const {
        return { x + (to.x - x) * T(t), y + (to.y - y) * T(t) };
    }

    // Angle of this vector in radians [-π, π]
    f32 Angle() const { return std::atan2(static_cast<f32>(y), static_cast<f32>(x)); }

    // Rotate by angle (radians)
    Vec2 Rotated(f32 angle) const {
        T c = static_cast<T>(std::cos(angle));
        T s = static_cast<T>(std::sin(angle));
        return { x * c - y * s, x * s + y * c };
    }

    // Debug
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v) {
        return os << "Vec2(" << v.x << ", " << v.y << ")";
    }
};

// Scalar on left: 2.0f * vec
template<typename T> constexpr Vec2<T> operator*(T s, const Vec2<T>& v) { return v * s; }
template<typename T> constexpr Vec2<T> operator+(T s, const Vec2<T>& v) { return v + s; }

// Convenience aliases
using Vec2f = Vec2<f32>;
using Vec2d = Vec2<f64>;
using Vec2i = Vec2<i32>;
using Vec2u = Vec2<u32>;

} // namespace Shape::Math

// Pull aliases into the Shape namespace for convenience
namespace Shape {
    using Math::Vec2f;
    using Math::Vec2i;
    using Math::Vec2u;
}
