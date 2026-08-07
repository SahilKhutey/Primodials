#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  Point3.hpp
//  A strongly-typed 3D location.
//
//  Design Rules:
//    - Point3 + Vector3 = Point3   (displacement)
//    - Point3 - Point3  = Vector3  (displacement vector)
//    - Point3 + Point3  = INVALID
//    - Point3 * Scalar  = INVALID
//
//  Satisfies: GeometryPrimitive, PointGeometry concepts.
// ─────────────────────────────────────────────────────────────────────────────

#include "ShapeEngine/Math/Geometry/Core/GeometryCore.hpp"
#include "ShapeEngine/Math/Vector3.hpp"
#include <cmath>

namespace Shape::Math {

template<typename T>
struct Point3 {
    T x = 0;
    T y = 0;
    T z = 0;

    // ── Geometry Framework Interface ──────────────────────────────────────────
    static constexpr Geometry::GeometryType GeomType  = Geometry::GeometryType::Point3;
    static constexpr u32                    Dim        = 3;
    static constexpr bool                   IsConvex   = true;
    static constexpr bool                   IsBounded  = true;
    static constexpr bool                   IsInfinite = false;

    [[nodiscard]] constexpr bool IsValid() const noexcept {
        return Geometry::IsValidScalar(x) &&
               Geometry::IsValidScalar(y) &&
               Geometry::IsValidScalar(z);
    }
    [[nodiscard]] constexpr bool IsFinite() const noexcept { return IsValid(); }
    [[nodiscard]] constexpr bool IsNormalized() const noexcept { return true; }

    // ── Constructors ──────────────────────────────────────────────────────────
    constexpr Point3() : x(0), y(0), z(0) {}
    constexpr Point3(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    constexpr explicit Point3(const Vector3<T>& v) : x(v.x), y(v.y), z(v.z) {}

    // ── Named Constructors ────────────────────────────────────────────────────
    static constexpr Point3 Origin() { return {}; }

    // ── Arithmetic ────────────────────────────────────────────────────────────
    constexpr Point3 operator+(const Vector3<T>& v) const noexcept {
        return { x + v.x, y + v.y, z + v.z };
    }
    constexpr Point3& operator+=(const Vector3<T>& v) noexcept {
        x += v.x; y += v.y; z += v.z; return *this;
    }

    constexpr Point3 operator-(const Vector3<T>& v) const noexcept {
        return { x - v.x, y - v.y, z - v.z };
    }
    constexpr Point3& operator-=(const Vector3<T>& v) noexcept {
        x -= v.x; y -= v.y; z -= v.z; return *this;
    }

    // Point - Point = displacement Vector
    constexpr Vector3<T> operator-(const Point3& other) const noexcept {
        return { x - other.x, y - other.y, z - other.z };
    }

    // ── Comparison ────────────────────────────────────────────────────────────
    constexpr bool operator==(const Point3& o) const noexcept {
        return x == o.x && y == o.y && z == o.z;
    }
    constexpr bool operator!=(const Point3& o) const noexcept { return !(*this == o); }

    [[nodiscard]] bool NearEquals(const Point3& o, T eps = Geometry::GeoEpsilon_v<T>) const noexcept {
        return Geometry::NearEqual(x, o.x, eps) &&
               Geometry::NearEqual(y, o.y, eps) &&
               Geometry::NearEqual(z, o.z, eps);
    }

    // ── Conversion ────────────────────────────────────────────────────────────
    [[nodiscard]] constexpr Vector3<T> AsVector() const noexcept { return { x, y, z }; }
};

// ── Free Functions ────────────────────────────────────────────────────────────
template<typename T>
inline T Distance(const Point3<T>& a, const Point3<T>& b) noexcept {
    T dx = b.x - a.x, dy = b.y - a.y, dz = b.z - a.z;
    return static_cast<T>(std::sqrt(static_cast<double>(dx*dx + dy*dy + dz*dz)));
}

template<typename T>
constexpr T DistanceSquared(const Point3<T>& a, const Point3<T>& b) noexcept {
    T dx = b.x - a.x, dy = b.y - a.y, dz = b.z - a.z;
    return dx*dx + dy*dy + dz*dz;
}

template<typename T>
constexpr Point3<T> Midpoint(const Point3<T>& a, const Point3<T>& b) noexcept {
    return { (a.x + b.x) * T(0.5),
             (a.y + b.y) * T(0.5),
             (a.z + b.z) * T(0.5) };
}

// Linear interpolation — valid for points (affine combination, t in [0,1])
template<typename T>
constexpr Point3<T> LerpPoint(const Point3<T>& a, const Point3<T>& b, T t) noexcept {
    return { a.x + (b.x - a.x) * t,
             a.y + (b.y - a.y) * t,
             a.z + (b.z - a.z) * t };
}

// Centroid of n points (valid — weighted affine combination, weights 1/n)
template<typename T>
inline Point3<T> Centroid(const Point3<T>* pts, u32 count) noexcept {
    if (count == 0) return Point3<T>::Origin();
    T inv = static_cast<T>(1) / static_cast<T>(count);
    T sx = 0, sy = 0, sz = 0;
    for (u32 i = 0; i < count; ++i) { sx += pts[i].x; sy += pts[i].y; sz += pts[i].z; }
    return { sx * inv, sy * inv, sz * inv };
}

// Transform a Point3 through TRS
template<typename T>
constexpr Point3<T> TransformPoint(
    const Point3<T>& point,
    const Vector3<T>& translation,
    const Quaternion<T>& rotation,
    const Vector3<T>& scale) noexcept
{
    Vector3<T> scaled = { point.x * scale.x, point.y * scale.y, point.z * scale.z };
    Vector3<T> rotated = rotation.Rotate(scaled);
    return { rotated.x + translation.x,
             rotated.y + translation.y,
             rotated.z + translation.z };
}

// ── Type Aliases ──────────────────────────────────────────────────────────────
using Point3f  = Point3<float>;
using Point3d  = Point3<double>;
using Point3i  = Point3<i32>;

} // namespace Shape::Math

// ── Traits Registration ───────────────────────────────────────────────────────
SHAPE_REGISTER_GEOM_TRAITS(
    Shape::Math::Point3f,
    3, true, true, false, false, false,
    Shape::Math::Geometry::GeometryCategory::Point
)
SHAPE_REGISTER_GEOM_TRAITS(
    Shape::Math::Point3d,
    3, true, true, false, false, false,
    Shape::Math::Geometry::GeometryCategory::Point
)
