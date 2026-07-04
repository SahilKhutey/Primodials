#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  GeometryConcepts.hpp
//  C++23 concepts constraining geometry primitive types.
//
//  Every primitive in the engine must satisfy one or more of these concepts.
//  Algorithms use these concepts to select the correct implementation path.
// ─────────────────────────────────────────────────────────────────────────────

#include "ShapeEngine/Math/Geometry/Core/GeometryType.hpp"
#include "ShapeEngine/Math/Geometry/Core/GeometryDimension.hpp"
#include <concepts>
#include <type_traits>

namespace Shape::Math::Geometry {

// ── Base Geometry Concept ─────────────────────────────────────────────────────
// Every geometry primitive must expose:
//   - static constexpr GeometryType  GeomType
//   - static constexpr u32           Dim  (spatial dimension)
//   - static constexpr bool          IsConvex
//   - static constexpr bool          IsBounded
//   - static constexpr bool          IsInfinite
//   - bool IsValid() const
//   - bool IsFinite() const

template<typename G>
concept GeometryPrimitive =
    requires {
        { G::GeomType  } -> std::convertible_to<GeometryType>;
        { G::Dim       } -> std::convertible_to<u32>;
        { G::IsConvex  } -> std::convertible_to<bool>;
        { G::IsBounded } -> std::convertible_to<bool>;
        { G::IsInfinite} -> std::convertible_to<bool>;
    } &&
    requires(const G& g) {
        { g.IsValid()  } -> std::convertible_to<bool>;
        { g.IsFinite() } -> std::convertible_to<bool>;
    };

// ── Point Concept ─────────────────────────────────────────────────────────────
// A point represents a location. It supports:
//   - operator+(VectorType) -> PointType
//   - operator-(PointType)  -> VectorType

template<typename G>
concept PointGeometry =
    GeometryPrimitive<G> &&
    (G::GeomType == GeometryType::Point2 ||
     G::GeomType == GeometryType::Point3 ||
     G::GeomType == GeometryType::Point4);

// ── Linear Concept ────────────────────────────────────────────────────────────
// Lines, rays, and segments expose:
//   - Evaluate(T t) -> PointType
//   - ClosestPoint(PointType) -> PointType

template<typename G, typename T>
concept LinearGeometry =
    GeometryPrimitive<G> &&
    requires(const G& g, T t) {
        { g.Evaluate(t) };
        { g.ClosestPoint(t) };
    };

// ── Bounded Concept ───────────────────────────────────────────────────────────
// Bounded geometry has finite extent, and can compute a bounding box.

template<typename G>
concept BoundedGeometry =
    GeometryPrimitive<G> &&
    G::IsBounded;

// ── Convex Concept ────────────────────────────────────────────────────────────
template<typename G>
concept ConvexGeometry =
    GeometryPrimitive<G> &&
    G::IsConvex;

// ── Scalar Type Concept ───────────────────────────────────────────────────────
template<typename T>
concept GeometryScalar =
    std::is_floating_point_v<T>;

} // namespace Shape::Math::Geometry
