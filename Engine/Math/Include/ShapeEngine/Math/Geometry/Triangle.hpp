#pragma once

#include "ShapeEngine/Math/Geometry/Point.hpp"

namespace Shape::Math {

template<typename T>
struct Triangle {
    Point3<T> a;
    Point3<T> b;
    Point3<T> c;

    constexpr Triangle() = default;
    constexpr Triangle(const Point3<T>& pA, const Point3<T>& pB, const Point3<T>& pC)
        : a(pA), b(pB), c(pC) {}

    inline T Area() const {
        return Length(Cross(b - a, c - a)) * static_cast<T>(0.5);
    }

    inline Vector3<T> Normal() const {
        return Normalize(Cross(b - a, c - a));
    }

    constexpr Point3<T> Centroid() const {
        return {
            (a.x + b.x + c.x) / static_cast<T>(3),
            (a.y + b.y + c.y) / static_cast<T>(3),
            (a.z + b.z + c.z) / static_cast<T>(3)
        };
    }

    // Closest point on triangle to external point (Ericson's method)
    constexpr Point3<T> ClosestPoint(const Point3<T>& p) const {
        Vector3<T> ab = b - a;
        Vector3<T> ac = c - a;
        Vector3<T> ap = p - a;

        T d1 = Dot(ab, ap);
        T d2 = Dot(ac, ap);
        if (d1 <= 0 && d2 <= 0) return a;

        Vector3<T> bp = p - b;
        T d3 = Dot(ab, bp);
        T d4 = Dot(ac, bp);
        if (d3 >= 0 && d4 <= d3) return b;

        T vc = d1 * d4 - d3 * d2;
        if (vc <= 0 && d1 >= 0 && d3 <= 0) {
            T v = d1 / (d1 - d3);
            return a + ab * v;
        }

        Vector3<T> cp = p - c;
        T d5 = Dot(ab, cp);
        T d6 = Dot(ac, cp);
        if (d6 >= 0 && d5 <= d6) return c;

        T vb = d5 * d2 - d1 * d6;
        if (vb <= 0 && d2 >= 0 && d6 <= 0) {
            T w = d2 / (d2 - d6);
            return a + ac * w;
        }

        T va = d3 * d6 - d5 * d4;
        if (va <= 0 && (d4 - d3) >= 0 && (d5 - d6) >= 0) {
            T w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
            Vector3<T> bc = c - b;
            return b + bc * w;
        }

        T denom = static_cast<T>(1) / (va + vb + vc);
        T v = vb * denom;
        T w = vc * denom;
        return a + ab * v + ac * w;
    }

    inline T Distance(const Point3<T>& point) const {
        return Math::Distance(point, ClosestPoint(point));
    }
};

using Triangle3f = Triangle<float>;

} // namespace Shape::Math
