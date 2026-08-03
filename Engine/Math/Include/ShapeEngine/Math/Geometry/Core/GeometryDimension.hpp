#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  GeometryDimension.hpp
//  Compile-time spatial dimension for every geometry primitive.
//
//  Usage:
//      static_assert(GeometryDimension<Point3f>::value == 3);
//      if constexpr (GeometryDimension<G>::value == 2) { ... }
// ─────────────────────────────────────────────────────────────────────────────

#include "Core/Platform.hpp"

namespace Shape::Math::Geometry {

// Primary template — must be specialized per primitive
template<typename G>
struct GeometryDimension {
    static constexpr u32 value = 3; // Default to 3D
};

// Helper alias
template<typename G>
inline constexpr u32 GeometryDimension_v = GeometryDimension<G>::value;

// Helper concept: G is a 2-D geometry type
template<typename G>
concept Is2DGeometry = (GeometryDimension_v<G> == 2);

// Helper concept: G is a 3-D geometry type
template<typename G>
concept Is3DGeometry = (GeometryDimension_v<G> == 3);

} // namespace Shape::Math::Geometry
