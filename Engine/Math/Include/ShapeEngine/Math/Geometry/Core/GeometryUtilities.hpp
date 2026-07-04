#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  GeometryUtilities.hpp
//  Shared utility operations used by all geometry algorithms.
//
//  Includes:
//    - NaN/Infinity detection
//    - Vector projection
//    - Scalar clamping and sign
//    - Epsilon helpers
//    - Triplet cross product (for plane/triangle)
// ─────────────────────────────────────────────────────────────────────────────

#include "ShapeEngine/Math/Geometry/Core/Precision.hpp"
#include "ShapeEngine/Math/Vector3.hpp"
#include <cmath>
#include <limits>

namespace Shape::Math::Geometry {

// ── Validity ──────────────────────────────────────────────────────────────────
// IsValidScalar and IsValidVec3 — delegates to Precision.hpp versions
template<typename T>
constexpr bool IsValidVec3(const Vector3<T>& v) noexcept {
    return IsValidScalar(v.x) && IsValidScalar(v.y) && IsValidScalar(v.z);
}

// ── Projection ────────────────────────────────────────────────────────────────
// Projects vector a onto vector b. Returns scalar parameter t.
template<typename T>
constexpr T ProjectScalar(const Vector3<T>& a, const Vector3<T>& b) noexcept {
    T lenSq = b.x * b.x + b.y * b.y + b.z * b.z;
    return (lenSq > PrecisionTraits<T>::SmallEps)
        ? (a.x * b.x + a.y * b.y + a.z * b.z) / lenSq
        : static_cast<T>(0);
}

// Projects a onto b returning a vector
template<typename T>
constexpr Vector3<T> Project(const Vector3<T>& a, const Vector3<T>& b) noexcept {
    return b * ProjectScalar(a, b);
}

// ── Rejection (perpendicular component) ───────────────────────────────────────
template<typename T>
constexpr Vector3<T> Reject(const Vector3<T>& a, const Vector3<T>& b) noexcept {
    return a - Project(a, b);
}

// ── Reflection ────────────────────────────────────────────────────────────────
// Reflects vector v about unit normal n.
template<typename T>
constexpr Vector3<T> Reflect(const Vector3<T>& v, const Vector3<T>& n) noexcept {
    T twoD = static_cast<T>(2) * (v.x * n.x + v.y * n.y + v.z * n.z);
    return { v.x - twoD * n.x, v.y - twoD * n.y, v.z - twoD * n.z };
}

// ── Refraction ────────────────────────────────────────────────────────────────
// Refracts direction d through surface with normal n, index of refraction ratio eta.
template<typename T>
inline Vector3<T> Refract(const Vector3<T>& d, const Vector3<T>& n, T eta) noexcept {
    T dot = d.x * n.x + d.y * n.y + d.z * n.z;
    T k   = static_cast<T>(1) - eta * eta * (static_cast<T>(1) - dot * dot);
    if (k < 0) return Vector3<T>(0, 0, 0); // Total internal reflection
    T scale = eta * dot + static_cast<T>(std::sqrt(static_cast<double>(k)));
    return { eta * d.x - scale * n.x,
             eta * d.y - scale * n.y,
             eta * d.z - scale * n.z };
}

// ── Scalar Utilities ──────────────────────────────────────────────────────────
template<typename T>
constexpr T SafeReciprocal(T v, T fallback = static_cast<T>(0)) noexcept {
    return (std::abs(static_cast<double>(v)) > static_cast<double>(PrecisionTraits<T>::SmallEps))
        ? static_cast<T>(1) / v
        : fallback;
}

template<typename T>
constexpr T Saturate(T v) noexcept {
    return v < static_cast<T>(0) ? static_cast<T>(0) :
           v > static_cast<T>(1) ? static_cast<T>(1) : v;
}

// ── Triangle Area (cross product magnitude) ───────────────────────────────────
template<typename T>
constexpr T TriangleAreaFromEdges(
    const Vector3<T>& ab,
    const Vector3<T>& ac) noexcept
{
    Vector3<T> cp = Math::Cross(ab, ac);
    return Math::Length(cp) * static_cast<T>(0.5);
}

// ── Signed Volume of Tetrahedron ──────────────────────────────────────────────
template<typename T>
constexpr T SignedTetraVolume(
    const Vector3<T>& a,
    const Vector3<T>& b,
    const Vector3<T>& c,
    const Vector3<T>& d) noexcept
{
    Vector3<T> ab = b - a;
    Vector3<T> ac = c - a;
    Vector3<T> ad = d - a;
    Vector3<T> cp = Math::Cross(ab, ac);
    return (cp.x * ad.x + cp.y * ad.y + cp.z * ad.z) / static_cast<T>(6);
}

// ── Barycentric Coordinates ───────────────────────────────────────────────────
// Computes barycentric coords (u, v, w) for point p relative to triangle (a, b, c).
// p = u*a + v*b + w*c,  u+v+w = 1
template<typename T>
constexpr Vector3<T> BarycentricCoords(
    const Vector3<T>& p,
    const Vector3<T>& a,
    const Vector3<T>& b,
    const Vector3<T>& c) noexcept
{
    Vector3<T> ab = b - a, ac = c - a, ap = p - a;
    T d00 = Math::Dot(ab, ab), d01 = Math::Dot(ab, ac);
    T d11 = Math::Dot(ac, ac), d20 = Math::Dot(ap, ab), d21 = Math::Dot(ap, ac);
    T inv = SafeReciprocal(d00 * d11 - d01 * d01);
    T v   = (d11 * d20 - d01 * d21) * inv;
    T w   = (d00 * d21 - d01 * d20) * inv;
    return { static_cast<T>(1) - v - w, v, w };
}

} // namespace Shape::Math::Geometry
