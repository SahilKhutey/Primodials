#pragma once

#include "ShapeEngine/Math/Geometry/Point.hpp"

namespace Shape::Math {

template<typename T>
struct AABB {
    Point3<T> minVal;
    Point3<T> maxVal;

    constexpr AABB() : minVal(0, 0, 0), maxVal(0, 0, 0) {}
    constexpr AABB(const Point3<T>& mn, const Point3<T>& mx) : minVal(mn), maxVal(mx) {}

    constexpr Point3<T> Center() const {
        return {
            (minVal.x + maxVal.x) * static_cast<T>(0.5),
            (minVal.y + maxVal.y) * static_cast<T>(0.5),
            (minVal.z + maxVal.z) * static_cast<T>(0.5)
        };
    }

    constexpr Vector3<T> Extent() const {
        return (maxVal - minVal) * static_cast<T>(0.5);
    }

    constexpr T Volume() const {
        Vector3<T> size = maxVal - minVal;
        return size.x * size.y * size.z;
    }

    constexpr bool Contains(const Point3<T>& point) const {
        return point.x >= minVal.x && point.x <= maxVal.x &&
               point.y >= minVal.y && point.y <= maxVal.y &&
               point.z >= minVal.z && point.z <= maxVal.z;
    }

    constexpr Point3<T> ClosestPoint(const Point3<T>& point) const {
        return {
            Clamp(point.x, minVal.x, maxVal.x),
            Clamp(point.y, minVal.y, maxVal.y),
            Clamp(point.z, minVal.z, maxVal.z)
        };
    }

    inline T Distance(const Point3<T>& point) const {
        return Math::Distance(point, ClosestPoint(point));
    }
};

using AABB3f = AABB<float>;

} // namespace Shape::Math
