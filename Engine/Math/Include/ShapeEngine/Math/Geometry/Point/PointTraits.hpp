#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  PointTraits.hpp
//  GeometryTraits specializations for all Point types.
//  GeometryDimension specializations for all Point types.
// ─────────────────────────────────────────────────────────────────────────────

#include "ShapeEngine/Math/Geometry/Point/Point2.hpp"
#include "ShapeEngine/Math/Geometry/Point/Point3.hpp"
#include "ShapeEngine/Math/Geometry/Point/Point4.hpp"
#include "ShapeEngine/Math/Geometry/Core/GeometryDimension.hpp"

namespace Shape::Math::Geometry {

// ── GeometryDimension Specializations ────────────────────────────────────────
template<typename T>
struct GeometryDimension<Math::Point2<T>> {
    static constexpr u32 value = 2;
};

template<typename T>
struct GeometryDimension<Math::Point3<T>> {
    static constexpr u32 value = 3;
};

template<typename T>
struct GeometryDimension<Math::Point4<T>> {
    static constexpr u32 value = 4;
};

// ── Static assertions: ensure all Point types satisfy GeometryPrimitive ───────
static_assert(Math::Point2f::IsConvex,   "Point2 must be convex");
static_assert(Math::Point2f::IsBounded,  "Point2 must be bounded");
static_assert(!Math::Point2f::IsInfinite,"Point2 must not be infinite");
static_assert(Math::Point2f::Dim == 2,   "Point2 must be 2D");

static_assert(Math::Point3f::IsConvex,   "Point3 must be convex");
static_assert(Math::Point3f::IsBounded,  "Point3 must be bounded");
static_assert(!Math::Point3f::IsInfinite,"Point3 must not be infinite");
static_assert(Math::Point3f::Dim == 3,   "Point3 must be 3D");

static_assert(Math::Point4f::IsConvex,   "Point4 must be convex");
static_assert(Math::Point4f::IsBounded,  "Point4 must be bounded");
static_assert(!Math::Point4f::IsInfinite,"Point4 must not be infinite");
static_assert(Math::Point4f::Dim == 4,   "Point4 must be 4D");

} // namespace Shape::Math::Geometry
