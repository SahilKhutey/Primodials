#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  Point4.hpp
//  A strongly-typed 4D homogeneous location.
//
//  Homogeneous coordinates: (x, y, z, w) where w=1 is a point, w=0 a direction.
//  Used in:
//    - Projective geometry (perspective division)
//    - Shader uniform upload
//    - Homogeneous clipping
//
//  Design Rules:
//    - Point4 + Vector4 = Point4   (valid)
//    - Point4 - Point4  = Vector4  (valid)
//    - Point4 + Point4  = INVALID  (not mathematical)
// ─────────────────────────────────────────────────────────────────────────────

#include "ShapeEngine/Math/Geometry/Core/GeometryCore.hpp"
#include "ShapeEngine/Math/Vector4.hpp"
#include "ShapeEngine/Math/Geometry/Point/Point3.hpp"

namespace Shape::Math {

template<typename T>
struct Point4 {
    T x = 0;
    T y = 0;
    T z = 0;
    T w = 1; // w=1 by default (Euclidean point)

    // ── Geometry Framework Interface ──────────────────────────────────────────
    static constexpr Geometry::GeometryType GeomType  = Geometry::GeometryType::Point4;
    static constexpr u32                    Dim        = 4;
    static constexpr bool                   IsConvex   = true;
    static constexpr bool                   IsBounded  = true;
    static constexpr bool                   IsInfinite = false;

    [[nodiscard]] constexpr bool IsValid() const noexcept {
        return Geometry::IsValidScalar(x) && Geometry::IsValidScalar(y) &&
               Geometry::IsValidScalar(z) && Geometry::IsValidScalar(w);
    }
    [[nodiscard]] constexpr bool IsFinite() const noexcept { return IsValid(); }
    [[nodiscard]] constexpr bool IsNormalized() const noexcept { return true; }

    // ── Constructors ──────────────────────────────────────────────────────────
    constexpr Point4() : x(0), y(0), z(0), w(1) {}
    constexpr Point4(T X, T Y, T Z, T W = static_cast<T>(1)) : x(X), y(Y), z(Z), w(W) {}

    // ── Named Constructors ────────────────────────────────────────────────────
    static constexpr Point4 Origin() { return {}; }
    static constexpr Point4 Infinity() { return { 0, 0, 0, 0 }; } // Point at infinity (w=0)

    // ── Homogeneous Operations ────────────────────────────────────────────────
    // Perspective divide: returns the Euclidean 3D representation
    [[nodiscard]] constexpr Point3<T> PerspectiveDivide() const noexcept {
        if (Geometry::IsValidScalar(w) && w != static_cast<T>(0)) {
            T inv = static_cast<T>(1) / w;
            return { x * inv, y * inv, z * inv };
        }
        return { x, y, z }; // Already at infinity
    }

    [[nodiscard]] constexpr bool IsAtInfinity() const noexcept {
        return w == static_cast<T>(0);
    }

    // ── Arithmetic ────────────────────────────────────────────────────────────
    constexpr Point4 operator+(const Vector4<T>& v) const noexcept {
        return { x + v.x, y + v.y, z + v.z, w + v.w };
    }
    constexpr Point4& operator+=(const Vector4<T>& v) noexcept {
        x += v.x; y += v.y; z += v.z; w += v.w; return *this;
    }

    constexpr Point4 operator-(const Vector4<T>& v) const noexcept {
        return { x - v.x, y - v.y, z - v.z, w - v.w };
    }

    // Point4 - Point4 = displacement Vector4
    constexpr Vector4<T> operator-(const Point4& o) const noexcept {
        return { x - o.x, y - o.y, z - o.z, w - o.w };
    }

    // ── Comparison ────────────────────────────────────────────────────────────
    constexpr bool operator==(const Point4& o) const noexcept {
        return x == o.x && y == o.y && z == o.z && w == o.w;
    }
    constexpr bool operator!=(const Point4& o) const noexcept { return !(*this == o); }

    // ── Conversion ────────────────────────────────────────────────────────────
    [[nodiscard]] constexpr Vector4<T> AsVector() const noexcept { return { x, y, z, w }; }
    [[nodiscard]] constexpr Point3<T>  ToPoint3() const noexcept { return PerspectiveDivide(); }
};

// ── Type Aliases ──────────────────────────────────────────────────────────────
using Point4f = Point4<float>;
using Point4d = Point4<double>;

} // namespace Shape::Math

// ── Traits Registration ───────────────────────────────────────────────────────
SHAPE_REGISTER_GEOM_TRAITS(
    Shape::Math::Point4f,
    4, true, true, false, false, false,
    Shape::Math::Geometry::GeometryCategory::Point
)
SHAPE_REGISTER_GEOM_TRAITS(
    Shape::Math::Point4d,
    4, true, true, false, false, false,
    Shape::Math::Geometry::GeometryCategory::Point
)
