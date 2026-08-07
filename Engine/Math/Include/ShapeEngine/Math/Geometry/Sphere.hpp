#pragma once

#include "ShapeEngine/Math/Geometry/Point.hpp"

namespace Shape::Math {

template<typename T>
struct Sphere {
    Point3<T> center;
    T radius = 0;

    constexpr Sphere() = default;
    constexpr Sphere(const Point3<T>& cent, T rad) : center(cent), radius(rad) {}

    constexpr T Volume() const {
        return static_cast<T>(4.1887902047) * radius * radius * radius; // 4/3 * PI * r^3
    }

    constexpr T SurfaceArea() const {
        return static_cast<T>(12.566370614) * radius * radius; // 4 * PI * r^2
    }

    inline bool Contains(const Point3<T>& point) const {
        return DistanceSquared(point, center) <= radius * radius;
    }

    inline Point3<T> ClosestPoint(const Point3<T>& point) const {
        Vector3<T> dir = point - center;
        T distSq = LengthSquared(dir);
        if (distSq <= radius * radius) {
            return point;
        }
        return center + Normalize(dir) * radius;
    }

    inline T Distance(const Point3<T>& point) const {
        T dist = Math::Distance(point, center) - radius;
        return dist < 0 ? 0 : dist;
    }
};

using Sphere3f = Sphere<float>;

} // namespace Shape::Math
