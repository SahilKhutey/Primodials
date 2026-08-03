#pragma once

#include "Core/Platform.hpp"
#include "ShapeEngine/Math/Vector2.hpp"
#include "ShapeEngine/Math/Vector3.hpp"

namespace Shape::Math {

template<typename T>
struct Point2 {
    T x = 0;
    T y = 0;

    constexpr Point2() : x(0), y(0) {}
    constexpr Point2(T X, T Y) : x(X), y(Y) {}

    constexpr Point2 operator+(const Vector2f& v) const {
        return {static_cast<T>(x + v.x), static_cast<T>(y + v.y)};
    }

    constexpr Point2 operator-(const Vector2f& v) const {
        return {static_cast<T>(x - v.x), static_cast<T>(y - v.y)};
    }

    constexpr Vector2f operator-(const Point2& other) const {
        return {static_cast<f32>(x - other.x), static_cast<f32>(y - other.y)};
    }

    constexpr bool operator==(const Point2& other) const {
        return x == other.x && y == other.y;
    }

    constexpr bool operator!=(const Point2& other) const {
        return !(*this == other);
    }
};

template<typename T>
struct Point3 {
    T x = 0;
    T y = 0;
    T z = 0;

    constexpr Point3() : x(0), y(0), z(0) {}
    constexpr Point3(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

    constexpr Point3 operator+(const Vector3<T>& v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    constexpr Point3 operator-(const Vector3<T>& v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    constexpr Vector3<T> operator-(const Point3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    constexpr bool operator==(const Point3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    constexpr bool operator!=(const Point3& other) const {
        return !(*this == other);
    }
};

template<typename T>
inline T DistanceSquared(const Point3<T>& a, const Point3<T>& b) {
    return LengthSquared(a - b);
}

template<typename T>
inline T Distance(const Point3<T>& a, const Point3<T>& b) {
    return Length(a - b);
}

using Point2f = Point2<float>;
using Point3f = Point3<float>;

} // namespace Shape::Math
