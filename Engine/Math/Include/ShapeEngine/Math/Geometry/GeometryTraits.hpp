#pragma once

#include "Core/Platform.hpp"

namespace Shape::Math {

enum class GeometryType {
    Point,
    Line,
    Ray,
    Segment,
    Plane,
    Triangle,
    Sphere,
    Capsule,
    AABB,
    OBB,
    Frustum
};

template<typename T>
struct GeometryTraits {
    static constexpr bool IsBounded = false;
    static constexpr bool IsConvex = true;
    static constexpr u32 Dimension = 3;
    static constexpr GeometryType Type = GeometryType::Point;
};

} // namespace Shape::Math
