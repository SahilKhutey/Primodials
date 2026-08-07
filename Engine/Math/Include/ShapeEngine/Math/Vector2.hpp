#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  Vector2.hpp  (new canonical header)
//  Templated 2D vector matching the convention of Vector3<T>.
//  Resides in ShapeEngine/Math/ — include from Math/Include.
// ─────────────────────────────────────────────────────────────────────────────

#include "Core/Platform.hpp"
#include "Math/MathUtils.hpp"
#include <cmath>

namespace Shape::Math {

template<typename T>
struct Vector2 {
    T x = 0;
    T y = 0;

    constexpr Vector2() : x(0), y(0) {}
    constexpr Vector2(T X, T Y) : x(X), y(Y) {}
    constexpr explicit Vector2(T scalar) : x(scalar), y(scalar) {}

    static constexpr Vector2 Zero()  { return {  0,  0 }; }
    static constexpr Vector2 One()   { return {  1,  1 }; }
    static constexpr Vector2 UnitX() { return {  1,  0 }; }
    static constexpr Vector2 UnitY() { return {  0,  1 }; }

    // ── Arithmetic ────────────────────────────────────────────────────────────
    constexpr Vector2 operator+(const Vector2& o) const { return { x + o.x, y + o.y }; }
    constexpr Vector2 operator-(const Vector2& o) const { return { x - o.x, y - o.y }; }
    constexpr Vector2 operator*(T s)              const { return { x * s,   y * s   }; }
    constexpr Vector2 operator/(T s)              const { T inv = 1/s; return { x * inv, y * inv }; }
    constexpr Vector2 operator-()                 const { return { -x, -y }; }

    constexpr Vector2& operator+=(const Vector2& o) { x += o.x; y += o.y; return *this; }
    constexpr Vector2& operator-=(const Vector2& o) { x -= o.x; y -= o.y; return *this; }
    constexpr Vector2& operator*=(T s)              { x *= s;   y *= s;   return *this; }

    constexpr bool operator==(const Vector2& o) const { return x == o.x && y == o.y; }
    constexpr bool operator!=(const Vector2& o) const { return !(*this == o); }

    // ── Length ────────────────────────────────────────────────────────────────
    constexpr T LengthSquared() const { return x * x + y * y; }
    T           Length()        const { return static_cast<T>(std::sqrt(static_cast<double>(LengthSquared()))); }

    Vector2 Normalized() const {
        T len = Length();
        return len > static_cast<T>(1e-10) ? Vector2{ x / len, y / len } : Zero();
    }
    void Normalize() { *this = Normalized(); }

    // ── Dot / Perp ────────────────────────────────────────────────────────────
    constexpr T Dot(const Vector2& o)  const { return x * o.x + y * o.y; }
    constexpr T Cross(const Vector2& o) const { return x * o.y - y * o.x; }  // 2D "cross" (scalar)

    // ── Lerp ─────────────────────────────────────────────────────────────────
    static constexpr Vector2 Lerp(const Vector2& a, const Vector2& b, T t) {
        return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
    }
};

// Free-function aliases for consistency with Vector3
template<typename T> constexpr T Dot(const Vector2<T>& a, const Vector2<T>& b) { return a.x*b.x + a.y*b.y; }
template<typename T> inline T Length(const Vector2<T>& v) { return v.Length(); }
template<typename T> inline Vector2<T> SafeNormalize(const Vector2<T>& v) { return v.Normalized(); }

using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
using Vector2i = Vector2<i32>;

} // namespace Shape::Math
