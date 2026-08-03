#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  Point2.hpp
//  A strongly-typed 2D location.
//
//  Design Rules:
//    - Point + Vector = Point   (displacement)
//    - Point - Point  = Vector  (difference)
//    - Point + Point  = INVALID (meaningless)
//    - Point * Scalar = INVALID (meaningless)
//
//  Satisfies: GeometryPrimitive, PointGeometry concepts.
// ─────────────────────────────────────────────────────────────────────────────

#include "ShapeEngine/Math/Geometry/Core/GeometryCore.hpp"
#include "ShapeEngine/Math/Vector2.hpp"
#include <cmath>

namespace Shape::Math {

template<typename T>
struct Point2 {
    T x = 0;
    T y = 0;

    // ── Geometry Framework Interface ──────────────────────────────────────────
    static constexpr Geometry::GeometryType GeomType  = Geometry::GeometryType::Point2;
    static constexpr u32                    Dim        = 2;
    static constexpr bool                   IsConvex   = true;
    static constexpr bool                   IsBounded  = true;
    static constexpr bool                   IsInfinite = false;

    [[nodiscard]] constexpr bool IsValid() const noexcept {
        return Geometry::IsValidScalar(x) && Geometry::IsValidScalar(y);
    }
    [[nodiscard]] constexpr bool IsFinite() const noexcept { return IsValid(); }
    [[nodiscard]] constexpr bool IsNormalized() const noexcept { return true; } // Points have no magnitude

    // ── Constructors ──────────────────────────────────────────────────────────
    constexpr Point2() : x(0), y(0) {}
    constexpr Point2(T X, T Y) : x(X), y(Y) {}
    constexpr explicit Point2(const Vector2<T>& v) : x(v.x), y(v.y) {}

    // ── Named Constructors ────────────────────────────────────────────────────
    static constexpr Point2 Origin() { return {}; }

    // ── Arithmetic (only mathematically valid operations) ─────────────────────
    // Point + Vector = displaced Point
    constexpr Point2 operator+(const Vector2<T>& v) const noexcept {
        return { x + v.x, y + v.y };
    }
    constexpr Point2& operator+=(const Vector2<T>& v) noexcept {
        x += v.x; y += v.y; return *this;
    }

    // Point - Vector = displaced Point (opposite direction)
    constexpr Point2 operator-(const Vector2<T>& v) const noexcept {
        return { x - v.x, y - v.y };
    }
    constexpr Point2& operator-=(const Vector2<T>& v) noexcept {
        x -= v.x; y -= v.y; return *this;
    }

    // Point - Point = displacement Vector
    constexpr Vector2<T> operator-(const Point2& other) const noexcept {
        return { x - other.x, y - other.y };
    }

    // ── Comparison ────────────────────────────────────────────────────────────
    constexpr bool operator==(const Point2& o) const noexcept {
        return x == o.x && y == o.y;
    }
    constexpr bool operator!=(const Point2& o) const noexcept { return !(*this == o); }

    [[nodiscard]] bool NearEquals(const Point2& o, T eps = Geometry::GeoEpsilon_v<T>) const noexcept {
        return Geometry::NearEqual(x, o.x, eps) && Geometry::NearEqual(y, o.y, eps);
    }

    // ── Conversion ────────────────────────────────────────────────────────────
    [[nodiscard]] constexpr Vector2<T> AsVector() const noexcept { return { x, y }; }
};

// ── Free Functions ────────────────────────────────────────────────────────────
template<typename T>
inline T Distance(const Point2<T>& a, const Point2<T>& b) noexcept {
    T dx = b.x - a.x, dy = b.y - a.y;
    return static_cast<T>(std::sqrt(static_cast<double>(dx*dx + dy*dy)));
}

template<typename T>
constexpr T DistanceSquared(const Point2<T>& a, const Point2<T>& b) noexcept {
    T dx = b.x - a.x, dy = b.y - a.y;
    return dx*dx + dy*dy;
}

template<typename T>
constexpr Point2<T> Midpoint(const Point2<T>& a, const Point2<T>& b) noexcept {
    return { (a.x + b.x) * T(0.5), (a.y + b.y) * T(0.5) };
}

// Affine combination (weighted sum — valid for points when weights sum to 1)
template<typename T>
constexpr Point2<T> AffineCombine(const Point2<T>& a, const Point2<T>& b, T t) noexcept {
    return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
}

// ── Type Aliases ──────────────────────────────────────────────────────────────
using Point2f  = Point2<float>;
using Point2d  = Point2<double>;
using Point2i  = Point2<i32>;

} // namespace Shape::Math

// ── Traits Registration ───────────────────────────────────────────────────────
SHAPE_REGISTER_GEOM_TRAITS(
    Shape::Math::Point2f,
    2, true, true, false, false, false,
    Shape::Math::Geometry::GeometryCategory::Point
)
SHAPE_REGISTER_GEOM_TRAITS(
    Shape::Math::Point2d,
    2, true, true, false, false, false,
    Shape::Math::Geometry::GeometryCategory::Point
)
