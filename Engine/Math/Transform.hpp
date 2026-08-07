#pragma once

#include "Math/Vec2.hpp"
#include "Math/Vec3.hpp"
#include "Math/Quat.hpp"

namespace Shape::Math {

// ─────────────────────────────────────────────────────────────────────────────
//  TTransform<T> — position + rotation (Quat) + scale.
//  Used everywhere an object has a location in the world.
// ─────────────────────────────────────────────────────────────────────────────
template<typename T>
struct TTransform {
    Vec3<T> position { T(0), T(0), T(0) };
    Quat<T> rotation { Quat<T>::Identity() };
    Vec3<T> scale    { T(1), T(1), T(1) };

    // Default — at origin, no rotation, unit scale
    constexpr TTransform() = default;

    // Convenience constructors
    explicit constexpr TTransform(const Vec3<T>& pos)
        : position(pos) {}

    constexpr TTransform(const Vec3<T>& pos, const Quat<T>& rot)
        : position(pos), rotation(rot) {}

    constexpr TTransform(const Vec3<T>& pos, const Quat<T>& rot, const Vec3<T>& scl)
        : position(pos), rotation(rot), scale(scl) {}

    // ── 2-D convenience ──────────────────────────────────────────────────────
    // For 2-D simulation the rotation is a single Z-axis angle.
    static TTransform From2D(Vec2<T> pos2d, T angleRad = T(0), T uniformScale = T(1)) {
        TTransform t;
        t.position = Vec3<T>{ pos2d.x, pos2d.y, T(0) };
        t.rotation = Quat<T>::FromAxisAngle(Vec3<T>::UnitZ(), angleRad);
        t.scale    = Vec3<T>{ uniformScale, uniformScale, T(1) };
        return t;
    }

    Vec2<T> Position2D() const { return { position.x, position.y }; }
    T       Angle2D()    const { return rotation.ToAngle2D(); }

    // ── World / Local transforms ──────────────────────────────────────────────
    // Transform a point from local space to world space.
    Vec3<T> TransformPoint(const Vec3<T>& local) const {
        Vec3<T> scaled = { local.x * scale.x, local.y * scale.y, local.z * scale.z };
        return position + rotation.Rotate(scaled);
    }

    // Transform a direction (no translation, no scale).
    Vec3<T> TransformDirection(const Vec3<T>& dir) const {
        return rotation.Rotate(dir);
    }

    // Transform a point from world space back to local space.
    Vec3<T> InverseTransformPoint(const Vec3<T>& world) const {
        Quat<T> inv    = rotation.Conjugate();
        Vec3<T> offset = world - position;
        Vec3<T> rotated = inv.Rotate(offset);
        return { rotated.x / scale.x, rotated.y / scale.y, rotated.z / scale.z };
    }

    // ── Composition ──────────────────────────────────────────────────────────
    // Combine two transforms (parent * child).
    TTransform operator*(const TTransform& child) const {
        TTransform result;
        result.scale    = { scale.x * child.scale.x,
                            scale.y * child.scale.y,
                            scale.z * child.scale.z };
        result.rotation = rotation * child.rotation;
        result.position = TransformPoint(child.position);
        return result;
    }

    // Inverse of this transform
    TTransform Inverse() const {
        TTransform inv;
        inv.rotation = rotation.Conjugate();
        inv.scale    = { T(1)/scale.x, T(1)/scale.y, T(1)/scale.z };
        inv.position = inv.rotation.Rotate({ -position.x * inv.scale.x,
                                             -position.y * inv.scale.y,
                                             -position.z * inv.scale.z });
        return inv;
    }

    // Linear interpolation (position + scale) with Slerp (rotation)
    static TTransform Lerp(const TTransform& a, const TTransform& b, T t) {
        TTransform r;
        r.position = a.position.Lerp(b.position, static_cast<f32>(t));
        r.rotation = Quat<T>::Slerp(a.rotation, b.rotation, t);
        r.scale    = a.scale.Lerp(b.scale, static_cast<f32>(t));
        return r;
    }

    bool operator==(const TTransform& o) const {
        return position == o.position && rotation == o.rotation && scale == o.scale;
    }
    bool operator!=(const TTransform& o) const { return !(*this == o); }
};

using Transform  = TTransform<f32>;
using Transformd = TTransform<f64>;

} // namespace Shape::Math

namespace Shape {
    using Math::Transform;
}
