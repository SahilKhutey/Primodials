#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  GeometryTraits.hpp
//  Compile-time property table for every geometry primitive.
//
//  Every specialization answers:
//    Dim         — spatial dimension
//    IsConvex    — is the shape convex?
//    IsBounded   — does the shape have finite extent?
//    IsInfinite  — does the shape extend to infinity?
//    HasVolume   — does the shape enclose a volume?
//    HasSurface  — does the shape have a surface?
//    Category    — GeometryCategory classification
//
//  Forward-declared here; specialized alongside each primitive header.
// ─────────────────────────────────────────────────────────────────────────────

#include "ShapeEngine/Math/Geometry/Core/GeometryType.hpp"

namespace Shape::Math::Geometry {

// ── Primary Template (must be explicitly specialized) ─────────────────────────
template<typename G>
struct GeometryTraits {
    static constexpr u32              Dim        = 3;
    static constexpr bool             IsConvex   = true;
    static constexpr bool             IsBounded  = true;
    static constexpr bool             IsInfinite = false;
    static constexpr bool             HasVolume  = false;
    static constexpr bool             HasSurface = false;
    static constexpr GeometryCategory Category   = GeometryCategory::Unknown;
};

// ── Helper Accessors ──────────────────────────────────────────────────────────
template<typename G>
inline constexpr u32  GeomDim_v        = GeometryTraits<G>::Dim;

template<typename G>
inline constexpr bool GeomIsConvex_v   = GeometryTraits<G>::IsConvex;

template<typename G>
inline constexpr bool GeomIsBounded_v  = GeometryTraits<G>::IsBounded;

template<typename G>
inline constexpr bool GeomIsInfinite_v = GeometryTraits<G>::IsInfinite;

template<typename G>
inline constexpr bool GeomHasVolume_v  = GeometryTraits<G>::HasVolume;

// ── Trait Registration Macro ──────────────────────────────────────────────────
// Simplifies registering traits for a geometry type.
//
//  SHAPE_REGISTER_GEOM_TRAITS(
//      Sphere3f,          // type
//      3,                 // dim
//      true,              // convex
//      true,              // bounded
//      false,             // infinite
//      true,              // has volume
//      true,              // has surface
//      GeometryCategory::Volume
//  )

#define SHAPE_REGISTER_GEOM_TRAITS(Type, DimVal, Convex, Bounded, Infinite, Volume, Surface, Cat) \
namespace Shape::Math::Geometry {                                                                  \
    template<> struct GeometryTraits<Type> {                                                       \
        static constexpr u32              Dim        = DimVal;                                     \
        static constexpr bool             IsConvex   = Convex;                                     \
        static constexpr bool             IsBounded  = Bounded;                                    \
        static constexpr bool             IsInfinite = Infinite;                                   \
        static constexpr bool             HasVolume  = Volume;                                     \
        static constexpr bool             HasSurface = Surface;                                    \
        static constexpr GeometryCategory Category   = Cat;                                        \
    };                                                                                             \
} // namespace Shape::Math::Geometry

} // namespace Shape::Math::Geometry
