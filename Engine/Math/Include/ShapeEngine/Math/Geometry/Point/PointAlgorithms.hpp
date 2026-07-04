#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  PointAlgorithms.hpp
//  Generic algorithms operating on Point types.
//  All functions are free-function templates.
//  None are implemented as member functions.
// ─────────────────────────────────────────────────────────────────────────────

#include "ShapeEngine/Math/Geometry/Point/Point2.hpp"
#include "ShapeEngine/Math/Geometry/Point/Point3.hpp"
#include "ShapeEngine/Math/Geometry/Point/Point4.hpp"
#include "ShapeEngine/Math/Geometry/Core/GeometryUtilities.hpp"
#include <cmath>

namespace Shape::Math::Geometry {

// ── Minimum Bounding Circle / Sphere ─────────────────────────────────────────
// Computes the circumscribed circle of three 2D points.
// Returns {center, radius}. Returns nullopt if degenerate.
template<typename T>
struct Circle2Result { Math::Point2<T> center; T radius; bool valid; };

template<typename T>
Circle2Result<T> Circumcircle(
    const Math::Point2<T>& a,
    const Math::Point2<T>& b,
    const Math::Point2<T>& c) noexcept
{
    T ax = b.x - a.x, ay = b.y - a.y;
    T bx = c.x - a.x, by = c.y - a.y;
    T D  = static_cast<T>(2) * (ax * by - ay * bx);

    if (std::abs(static_cast<double>(D)) < static_cast<double>(GeoEpsilon_v<T>)) {
        return { {}, {}, false };
    }
    T ux = (by * (ax*ax + ay*ay) - ay * (bx*bx + by*by)) / D;
    T uy = (ax * (bx*bx + by*by) - bx * (ax*ax + ay*ay)) / D;
    Math::Point2<T> center{ a.x + ux, a.y + uy };
    T radius = Math::Distance(center, a);
    return { center, radius, true };
}

// ── Point Cloud Bounding Box ──────────────────────────────────────────────────
template<typename T>
struct BBox3Result { Math::Point3<T> minPt; Math::Point3<T> maxPt; };

template<typename T>
BBox3Result<T> BoundingBox(const Math::Point3<T>* pts, u32 count) noexcept {
    if (count == 0) return { Math::Point3<T>::Origin(), Math::Point3<T>::Origin() };
    Math::Point3<T> mn = pts[0], mx = pts[0];
    for (u32 i = 1; i < count; ++i) {
        if (pts[i].x < mn.x) mn.x = pts[i].x;
        if (pts[i].y < mn.y) mn.y = pts[i].y;
        if (pts[i].z < mn.z) mn.z = pts[i].z;
        if (pts[i].x > mx.x) mx.x = pts[i].x;
        if (pts[i].y > mx.y) mx.y = pts[i].y;
        if (pts[i].z > mx.z) mx.z = pts[i].z;
    }
    return { mn, mx };
}

// ── Ritter's Bounding Sphere (fast, approximate) ─────────────────────────────
template<typename T>
struct BSphereResult { Math::Point3<T> center; T radius; };

template<typename T>
BSphereResult<T> RitterSphere(const Math::Point3<T>* pts, u32 count) noexcept {
    if (count == 0) return { Math::Point3<T>::Origin(), static_cast<T>(0) };
    Math::Point3<T> center = pts[0];
    T radius = static_cast<T>(0);

    // Pass 1: grow sphere
    for (u32 i = 1; i < count; ++i) {
        T d = Math::Distance(center, pts[i]);
        if (d > radius) {
            T half = (d - radius) * static_cast<T>(0.5);
            radius += half;
            Vector3<T> dir = SafeNormalize((pts[i] - center));
            center += dir * half;
        }
    }
    // Pass 2: guarantee containment
    for (u32 i = 0; i < count; ++i) {
        T d = Math::Distance(center, pts[i]);
        if (d > radius) radius = d;
    }
    return { center, radius };
}

// ── Convex Hull (Gift-Wrapping, 2D) ──────────────────────────────────────────
// Returns indices of hull points in CCW order.
// O(n*h) where h = hull size.
template<typename T>
u32 ConvexHull2D(
    const Math::Point2<T>* pts,
    u32 count,
    u32* outIndices) noexcept
{
    if (count < 3) return 0;

    // Start from the leftmost point
    u32 start = 0;
    for (u32 i = 1; i < count; ++i) {
        if (pts[i].x < pts[start].x) start = i;
    }

    u32 current = start, hull = 0;
    do {
        outIndices[hull++] = current;
        u32 next = (current + 1) % count;
        for (u32 i = 0; i < count; ++i) {
            // Cross product sign determines left turn
            T dx1 = pts[next].x - pts[current].x;
            T dy1 = pts[next].y - pts[current].y;
            T dx2 = pts[i].x   - pts[current].x;
            T dy2 = pts[i].y   - pts[current].y;
            if (dx1 * dy2 - dy1 * dx2 < 0) next = i;
        }
        current = next;
        if (hull >= count) break; // Safety guard
    } while (current != start);

    return hull;
}

// ── Closest Pair of Points (2D brute force) ───────────────────────────────────
template<typename T>
struct ClosestPairResult { u32 indexA; u32 indexB; T distanceSquared; };

template<typename T>
ClosestPairResult<T> ClosestPair2D(
    const Math::Point2<T>* pts,
    u32 count) noexcept
{
    if (count < 2) return { 0, 0, static_cast<T>(0) };
    T minDist = Math::DistanceSquared(pts[0], pts[1]);
    u32 bestA = 0, bestB = 1;
    for (u32 i = 0; i < count; ++i) {
        for (u32 j = i + 1; j < count; ++j) {
            T d = Math::DistanceSquared(pts[i], pts[j]);
            if (d < minDist) { minDist = d; bestA = i; bestB = j; }
        }
    }
    return { bestA, bestB, minDist };
}

} // namespace Shape::Math::Geometry
