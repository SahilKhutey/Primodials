#pragma once

#include "ShapeEngine/Math/Geometry/Plane.hpp"
#include "ShapeEngine/Math/Geometry/AABB.hpp"
#include "ShapeEngine/Math/Geometry/Sphere.hpp"

namespace Shape::Math {

template<typename T>
struct Frustum {
    // Planes: [0]=Near [1]=Far [2]=Left [3]=Right [4]=Bottom [5]=Top
    Plane<T> planes[6];

    constexpr Frustum() = default;

    // Classify a point — positive = inside, negative = outside
    int ClassifyPoint(const Point3<T>& point) const {
        for (int i = 0; i < 6; ++i) {
            if (planes[i].SignedDistance(point) < 0) {
                return -1; // outside
            }
        }
        return 1; // inside
    }

    bool ContainsPoint(const Point3<T>& point) const {
        return ClassifyPoint(point) >= 0;
    }

    bool ContainsSphere(const Sphere<T>& sphere) const {
        for (int i = 0; i < 6; ++i) {
            if (planes[i].SignedDistance(sphere.center) < -sphere.radius) {
                return false;
            }
        }
        return true;
    }

    bool ContainsAABB(const AABB<T>& aabb) const {
        Vector3<T> ext = aabb.Extent();
        Point3<T>  cen = aabb.Center();

        for (int i = 0; i < 6; ++i) {
            const Vector3<T>& n = planes[i].normal;
            // Positive extent projection along plane normal
            T radius = Abs(n.x) * ext.x + Abs(n.y) * ext.y + Abs(n.z) * ext.z;
            T dist   = planes[i].SignedDistance(cen);
            if (dist < -radius) {
                return false;
            }
        }
        return true;
    }
};

using Frustum3f = Frustum<float>;

} // namespace Shape::Math
