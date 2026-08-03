#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  GeometryType.hpp
//  Canonical enumeration of every geometry primitive in the engine.
//  All primitives expose:
//      static constexpr GeometryType GeomType;
// ─────────────────────────────────────────────────────────────────────────────

#include "Core/Platform.hpp"

namespace Shape::Math::Geometry {

enum class GeometryType : u8 {
    Unknown  = 0,

    // Points
    Point2,
    Point3,
    Point4,

    // Linear
    Line,
    Ray,
    Segment,

    // Surfaces
    Plane,
    Triangle,
    Circle,
    Disc,
    Polygon,

    // Volumes
    Sphere,
    Capsule,
    Cylinder,
    Cone,

    // Bounding Volumes
    AABB,
    OBB,

    // View / Culling
    Frustum,

    // Complex
    ConvexHull,
    Mesh,
};

enum class GeometryCategory : u8 {
    Unknown   = 0,
    Point     = 1,  // Zero-dimensional location
    Linear    = 2,  // Lines, rays, segments
    Surface   = 3,  // Planes, triangles, polygons
    Volume    = 4,  // Spheres, capsules, AABB, OBB
    Infinite  = 5,  // Planes, Lines (extend to infinity)
    Composite = 6,  // Meshes, convex hulls
};

// ── Geometry Category from Type ───────────────────────────────────────────────
constexpr GeometryCategory CategoryOf(GeometryType type) noexcept {
    switch (type) {
        case GeometryType::Point2:
        case GeometryType::Point3:
        case GeometryType::Point4:    return GeometryCategory::Point;

        case GeometryType::Line:
        case GeometryType::Ray:
        case GeometryType::Segment:   return GeometryCategory::Linear;

        case GeometryType::Plane:
        case GeometryType::Triangle:
        case GeometryType::Circle:
        case GeometryType::Disc:
        case GeometryType::Polygon:   return GeometryCategory::Surface;

        case GeometryType::Sphere:
        case GeometryType::Capsule:
        case GeometryType::Cylinder:
        case GeometryType::Cone:
        case GeometryType::AABB:
        case GeometryType::OBB:       return GeometryCategory::Volume;

        case GeometryType::Frustum:
        case GeometryType::ConvexHull:
        case GeometryType::Mesh:      return GeometryCategory::Composite;

        default:                      return GeometryCategory::Unknown;
    }
}

} // namespace Shape::Math::Geometry
