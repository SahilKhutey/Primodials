#pragma once

#include "ShapeEngine/Math/Geometry/Point.hpp"

namespace Shape::Math {

template<typename T>
struct Ray {
    Point3<T> origin;
    Vector3<T> direction; // Assumed normalized

    constexpr Ray() = default;
    constexpr Ray(const Point3<T>& orig, const Vector3<T>& dir) : origin(orig), direction(dir) {}

    constexpr Point3<T> Evaluate(T t) const {
        return origin + direction * (t < 0 ? 0 : t);
    }

    constexpr Point3<T> ClosestPoint(const Point3<T>& point) const {
        Vector3<T> op = point - origin;
        T t = Dot(op, direction);
        if (t < 0) {
            t = 0;
        }
        return Evaluate(t);
    }

    inline T Distance(const Point3<T>& point) const {
        return Math::Distance(point, ClosestPoint(point));
    }
};

using Ray3f = Ray<float>;

} // namespace Shape::Math
