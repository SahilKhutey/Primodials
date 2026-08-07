#pragma once

#include "ShapeEngine/Math/Geometry/Point.hpp"

namespace Shape::Math {

template<typename T>
struct Line {
    Point3<T> origin;
    Vector3<T> direction; // Assumed normalized

    constexpr Line() = default;
    constexpr Line(const Point3<T>& orig, const Vector3<T>& dir) : origin(orig), direction(dir) {}

    constexpr Point3<T> Evaluate(T t) const {
        return origin + direction * t;
    }

    constexpr Point3<T> ClosestPoint(const Point3<T>& point) const {
        Vector3<T> op = point - origin;
        T t = Dot(op, direction);
        return Evaluate(t);
    }

    inline T Distance(const Point3<T>& point) const {
        return Math::Distance(point, ClosestPoint(point));
    }
};

using Line3f = Line<float>;

} // namespace Shape::Math
