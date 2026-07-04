#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  Vector4.hpp  (new canonical header)
//  Templated 4D vector matching the convention of Vector3<T>.
//  Resides in ShapeEngine/Math/ — include from Math/Include.
// ─────────────────────────────────────────────────────────────────────────────

#include "Core/Platform.hpp"
#include "ShapeEngine/Math/Vector3.hpp"
#include <cmath>

namespace Shape::Math {

template<typename T>
struct Vector4 {
    T x = 0;
    T y = 0;
    T z = 0;
    T w = 0;

    constexpr Vector4() : x(0), y(0), z(0), w(0) {}
    constexpr Vector4(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}
    constexpr explicit Vector4(T scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
    constexpr Vector4(const Vector3<T>& xyz, T W) : x(xyz.x), y(xyz.y), z(xyz.z), w(W) {}

    static constexpr Vector4 Zero()  { return { 0, 0, 0, 0 }; }
    static constexpr Vector4 One()   { return { 1, 1, 1, 1 }; }

    // ── Arithmetic ────────────────────────────────────────────────────────────
    constexpr Vector4 operator+(const Vector4& o) const { return { x+o.x, y+o.y, z+o.z, w+o.w }; }
    constexpr Vector4 operator-(const Vector4& o) const { return { x-o.x, y-o.y, z-o.z, w-o.w }; }
    constexpr Vector4 operator*(T s)              const { return { x*s,   y*s,   z*s,   w*s   }; }
    constexpr Vector4 operator-()                 const { return { -x, -y, -z, -w }; }

    constexpr Vector4& operator+=(const Vector4& o) { x+=o.x; y+=o.y; z+=o.z; w+=o.w; return *this; }
    constexpr Vector4& operator-=(const Vector4& o) { x-=o.x; y-=o.y; z-=o.z; w-=o.w; return *this; }
    constexpr Vector4& operator*=(T s)              { x*=s;   y*=s;   z*=s;   w*=s;   return *this; }

    constexpr bool operator==(const Vector4& o) const { return x==o.x && y==o.y && z==o.z && w==o.w; }
    constexpr bool operator!=(const Vector4& o) const { return !(*this == o); }

    // ── Length / Dot ──────────────────────────────────────────────────────────
    constexpr T LengthSquared() const { return x*x + y*y + z*z + w*w; }
    T           Length()        const { return static_cast<T>(std::sqrt(static_cast<double>(LengthSquared()))); }

    constexpr T Dot(const Vector4& o) const { return x*o.x + y*o.y + z*o.z + w*o.w; }

    Vector4 Normalized() const {
        T len = Length();
        return len > static_cast<T>(1e-10) ? Vector4{ x/len, y/len, z/len, w/len } : Zero();
    }

    // ── Swizzle ───────────────────────────────────────────────────────────────
    constexpr Vector3<T> XYZ() const { return { x, y, z }; }
};

using Vector4f = Vector4<float>;
using Vector4d = Vector4<double>;

} // namespace Shape::Math
