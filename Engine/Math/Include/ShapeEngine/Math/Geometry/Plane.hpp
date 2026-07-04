#pragma once

#include "ShapeEngine/Math/Geometry/Point.hpp"

namespace Shape::Math {

// Helper: dot a Point3 position vector against a normal
template<typename T>
constexpr T DotPosNorm(const Point3<T>& pt, const Vector3<T>& n) {
    return pt.x * n.x + pt.y * n.y + pt.z * n.z;
}

template<typename T>
struct Plane {
    Vector3<T> normal;
    T distance = 0; // Ax + By + Cz + D = 0  where D = distance (negative dot)

    constexpr Plane() : normal(0, 1, 0), distance(0) {}
    constexpr Plane(const Vector3<T>& norm, T dist) : normal(norm), distance(dist) {}
    constexpr Plane(const Point3<T>& point, const Vector3<T>& norm)
        : normal(norm), distance(-DotPosNorm(point, norm)) {}

    constexpr Plane(const Point3<T>& pA, const Point3<T>& pB, const Point3<T>& pC) {
        Vector3<T> ab = pB - pA;
        Vector3<T> ac = pC - pA;
        normal   = Normalize(Cross(ab, ac));
        distance = -DotPosNorm(pA, normal);
    }

    constexpr T SignedDistance(const Point3<T>& point) const {
        return DotPosNorm(point, normal) + distance;
    }

    constexpr Point3<T> ProjectPoint(const Point3<T>& point) const {
        T sd = SignedDistance(point);
        return { point.x - normal.x * sd,
                 point.y - normal.y * sd,
                 point.z - normal.z * sd };
    }

    constexpr Point3<T> ClosestPoint(const Point3<T>& point) const {
        return ProjectPoint(point);
    }
};

using Plane3f = Plane<float>;

} // namespace Shape::Math
