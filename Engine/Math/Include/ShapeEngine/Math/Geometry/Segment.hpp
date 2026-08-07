#pragma once

#include "ShapeEngine/Math/Geometry/Point.hpp"

namespace Shape::Math {

template<typename T>
struct Segment {
    Point3<T> a;
    Point3<T> b;

    constexpr Segment() = default;
    constexpr Segment(const Point3<T>& pA, const Point3<T>& pB) : a(pA), b(pB) {}

    inline T Length() const {
        return Math::Distance(a, b);
    }

    inline T LengthSquared() const {
        return DistanceSquared(a, b);
    }

    constexpr Vector3<T> Direction() const {
        return Normalize(b - a);
    }

    constexpr Point3<T> MidPoint() const {
        return a + (b - a) * static_cast<T>(0.5);
    }

    constexpr Point3<T> ClosestPoint(const Point3<T>& point) const {
        Vector3<T> ab = b - a;
        T abLenSq = Dot(ab, ab);
        if (abLenSq == 0) return a;

        Vector3<T> ap = point - a;
        T t = Dot(ap, ab) / abLenSq;
        if (t < 0) t = 0;
        if (t > 1) t = 1;

        return a + ab * t;
    }

    inline T Distance(const Point3<T>& point) const {
        return Math::Distance(point, ClosestPoint(point));
    }
};

using Segment3f = Segment<float>;

} // namespace Shape::Math
