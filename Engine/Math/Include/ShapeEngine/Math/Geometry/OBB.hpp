#pragma once

#include "ShapeEngine/Math/Geometry/AABB.hpp"
#include "ShapeEngine/Math/Quaternion.hpp"

namespace Shape::Math {

template<typename T>
struct OBB {
    Vector3<T> center;
    Vector3<T> halfExtents;
    Quaternion<T> orientation;

    constexpr OBB() : center(0,0,0), halfExtents(1,1,1), orientation(Quaternion<T>::Identity()) {}
    constexpr OBB(const Vector3<T>& cen, const Vector3<T>& half, const Quaternion<T>& rot)
        : center(cen), halfExtents(half), orientation(rot) {}

    // Convert world-space point to OBB local space
    constexpr Vector3<T> ToLocal(const Vector3<T>& worldPoint) const {
        return orientation.Conjugate().Rotate(worldPoint - center);
    }

    constexpr bool Contains(const Vector3<T>& worldPoint) const {
        Vector3<T> local = ToLocal(worldPoint);
        return Abs(local.x) <= halfExtents.x &&
               Abs(local.y) <= halfExtents.y &&
               Abs(local.z) <= halfExtents.z;
    }

    constexpr Vector3<T> ClosestPoint(const Vector3<T>& worldPoint) const {
        Vector3<T> local = ToLocal(worldPoint);
        Vector3<T> clamped = {
            Clamp(local.x, -halfExtents.x, halfExtents.x),
            Clamp(local.y, -halfExtents.y, halfExtents.y),
            Clamp(local.z, -halfExtents.z, halfExtents.z)
        };
        return center + orientation.Rotate(clamped);
    }

    inline T Distance(const Vector3<T>& worldPoint) const {
        return Length(worldPoint - ClosestPoint(worldPoint));
    }
};

using OBB3f = OBB<float>;

} // namespace Shape::Math
