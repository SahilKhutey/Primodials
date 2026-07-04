#pragma once

#include "ShapeEngine/Math/Geometry/Segment.hpp"

namespace Shape::Math {

template<typename T>
struct Capsule {
    Segment<T> axis;
    T radius = 0;

    constexpr Capsule() = default;
    constexpr Capsule(const Segment<T>& ax, T rad) : axis(ax), radius(rad) {}

    constexpr Point3<T> ClosestPoint(const Point3<T>& point) const {
        Point3<T> segmentPoint = axis.ClosestPoint(point);
        Vector3<T> dir = point - segmentPoint;
        T distSq = LengthSquared(dir);
        if (distSq <= radius * radius) {
            return point;
        }
        return segmentPoint + Normalize(dir) * radius;
    }

    inline T Distance(const Point3<T>& point) const {
        T dist = (axis - point).Length() - radius;
        return dist < 0 ? 0 : dist;
    }
};

using Capsule3f = Capsule<float>;

} // namespace Shape::Math
