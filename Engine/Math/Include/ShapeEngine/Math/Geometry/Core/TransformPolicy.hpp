#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  TransformPolicy.hpp
//  Defines how geometry primitives are transformed through space.
//
//  A transform policy decouples the "how" from the "what" — a primitive
//  does not need to know whether it's being transformed via a Matrix4,
//  a Quaternion, or a full TRS Transform. The policy handles the dispatch.
//
//  Usage:
//      auto transformed = TransformPolicy<Matrix4Transform>::Apply(sphere, mat);
//      auto transformed = TransformPolicy<TRSTransform>::Apply(ray, xf);
// ─────────────────────────────────────────────────────────────────────────────

#include "Core/Platform.hpp"
#include "ShapeEngine/Math/Vector3.hpp"
#include "ShapeEngine/Math/Quaternion.hpp"

namespace Shape::Math::Geometry {

// ── Transform Tags ────────────────────────────────────────────────────────────
struct IdentityTransformTag {};     // No transformation
struct TRSTransformTag {};          // Position + Rotation + Scale
struct QuaternionTransformTag {};   // Rotation only

// ── Helper: transform a point through TRS ─────────────────────────────────────
template<typename T>
constexpr Vector3<T> TransformPoint(
    const Vector3<T>& point,
    const Vector3<T>& translation,
    const Quaternion<T>& rotation,
    const Vector3<T>& scale) noexcept
{
    Vector3<T> scaled = { point.x * scale.x, point.y * scale.y, point.z * scale.z };
    return translation + rotation.Rotate(scaled);
}

// ── Helper: transform a direction (ignores translation) ───────────────────────
template<typename T>
constexpr Vector3<T> TransformDirection(
    const Vector3<T>& dir,
    const Quaternion<T>& rotation) noexcept
{
    return rotation.Rotate(dir);
}

// ── Helper: transform a normal (inverse-transpose for non-uniform scale) ───────
template<typename T>
inline Vector3<T> TransformNormal(
    const Vector3<T>& normal,
    const Quaternion<T>& rotation,
    const Vector3<T>& scale) noexcept
{
    Vector3<T> invScale = {
        scale.x != 0 ? static_cast<T>(1) / scale.x : static_cast<T>(0),
        scale.y != 0 ? static_cast<T>(1) / scale.y : static_cast<T>(0),
        scale.z != 0 ? static_cast<T>(1) / scale.z : static_cast<T>(0)
    };
    return SafeNormalize(rotation.Rotate(
        Vector3<T>{ normal.x * invScale.x, normal.y * invScale.y, normal.z * invScale.z }
    ));
}

} // namespace Shape::Math::Geometry
