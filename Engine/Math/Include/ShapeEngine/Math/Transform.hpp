#pragma once

#include "ShapeEngine/Math/Quaternion.hpp"

namespace Shape::Math {

template<typename T>
struct Transform {
    Vector3<T> position;
    Quaternion<T> rotation;
    Vector3<T> scale;

    // ── Constructors ──────────────────────────────────────────────────────────
    constexpr Transform()
        : position(0, 0, 0), rotation(Quaternion<T>::Identity()), scale(1, 1, 1) {}

    constexpr Transform(const Vector3<T>& pos, const Quaternion<T>& rot, const Vector3<T>& scl)
        : position(pos), rotation(rot), scale(scl) {}

    static constexpr Transform Identity() {
        return {};
    }

    // ── Transform Point Operations ────────────────────────────────────────────
    Vector3<T> TransformPoint(const Vector3<T>& point) const {
        Vector3<T> scaled = { point.x * scale.x, point.y * scale.y, point.z * scale.z };
        return position + rotation.Rotate(scaled);
    }

    Vector3<T> TransformDirection(const Vector3<T>& dir) const {
        return rotation.Rotate(dir);
    }

    // Normals require inverse-transpose (for non-uniform scale)
    Vector3<T> TransformNormal(const Vector3<T>& normal) const {
        Vector3<T> invScale = {
            scale.x != 0 ? static_cast<T>(1) / scale.x : 0,
            scale.y != 0 ? static_cast<T>(1) / scale.y : 0,
            scale.z != 0 ? static_cast<T>(1) / scale.z : 0
        };
        return SafeNormalize(rotation.Rotate(Vector3<T>{
            normal.x * invScale.x,
            normal.y * invScale.y,
            normal.z * invScale.z
        }));
    }

    Vector3<T> InverseTransformPoint(const Vector3<T>& point) const {
        Vector3<T> local = rotation.Conjugate().Rotate(point - position);
        return {
            scale.x != 0 ? local.x / scale.x : 0,
            scale.y != 0 ? local.y / scale.y : 0,
            scale.z != 0 ? local.z / scale.z : 0
        };
    }

    // ── Composition ───────────────────────────────────────────────────────────
    static Transform Compose(const Transform& parent, const Transform& child) {
        return {
            parent.TransformPoint(child.position),
            parent.rotation * child.rotation,
            {
                parent.scale.x * child.scale.x,
                parent.scale.y * child.scale.y,
                parent.scale.z * child.scale.z
            }
        };
    }

    Transform Inverse() const {
        Quaternion<T> invRot = rotation.Conjugate();
        Vector3<T> invScale = {
            scale.x != 0 ? static_cast<T>(1) / scale.x : 0,
            scale.y != 0 ? static_cast<T>(1) / scale.y : 0,
            scale.z != 0 ? static_cast<T>(1) / scale.z : 0
        };
        Vector3<T> invPos = invRot.Rotate(Vector3<T>{
            -position.x * invScale.x,
            -position.y * invScale.y,
            -position.z * invScale.z
        });
        return { invPos, invRot, invScale };
    }

    // ── Interpolation ─────────────────────────────────────────────────────────
    static Transform Lerp(const Transform& a, const Transform& b, T t) {
        return {
            Vector3<T>::Lerp(a.position, b.position, t),
            Quaternion<T>::SLerp(a.rotation, b.rotation, t),
            Vector3<T>::Lerp(a.scale, b.scale, t)
        };
    }

    static Transform NLerp(const Transform& a, const Transform& b, T t) {
        return {
            Vector3<T>::Lerp(a.position, b.position, t),
            Quaternion<T>::NLerp(a.rotation, b.rotation, t),
            Vector3<T>::Lerp(a.scale, b.scale, t)
        };
    }
};

using Transformf = Transform<float>;
using Transformd = Transform<double>;

} // namespace Shape::Math
