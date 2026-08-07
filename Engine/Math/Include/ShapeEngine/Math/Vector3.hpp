#pragma once

#include "Core/Platform.hpp"
#include "Math/MathUtils.hpp"
#include <cmath>
#include <string>
#include <functional>
#include <algorithm>

namespace Shape::Math {

template<typename T>
struct Vector3 {
    T x = 0;
    T y = 0;
    T z = 0;

    constexpr Vector3() : x(0), y(0), z(0) {}
    constexpr Vector3(T value) : x(value), y(value), z(value) {}
    constexpr Vector3(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

    // Static Constants
    static constexpr Vector3 Zero()     { return {0, 0, 0}; }
    static constexpr Vector3 One()      { return {1, 1, 1}; }
    static constexpr Vector3 UnitX()    { return {1, 0, 0}; }
    static constexpr Vector3 UnitY()    { return {0, 1, 0}; }
    static constexpr Vector3 UnitZ()    { return {0, 0, 1}; }
    static constexpr Vector3 Forward()  { return {0, 0, 1}; }
    static constexpr Vector3 Backward() { return {0, 0, -1}; }
    static constexpr Vector3 Right()    { return {1, 0, 0}; }
    static constexpr Vector3 Left()     { return {-1, 0, 0}; }
    static constexpr Vector3 Up()       { return {0, 1, 0}; }
    static constexpr Vector3 Down()     { return {0, -1, 0}; }

    // Operator overloading
    constexpr T& operator[](usize index) {
        if (index == 0) return x;
        if (index == 1) return y;
        return z;
    }

    constexpr const T& operator[](usize index) const {
        if (index == 0) return x;
        if (index == 1) return y;
        return z;
    }

    constexpr bool operator==(const Vector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    constexpr bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }

    constexpr Vector3 operator+(const Vector3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    constexpr Vector3 operator-(const Vector3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    constexpr Vector3 operator*(T scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    constexpr Vector3 operator/(T scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }

    constexpr Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    constexpr Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    constexpr Vector3& operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    constexpr Vector3& operator/=(T scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    static constexpr Vector3 Lerp(const Vector3& a, const Vector3& b, T t) {
        return a + (b - a) * t;
    }
};

// Global Operator Overloads
template<typename T>
constexpr Vector3<T> operator*(T scalar, const Vector3<T>& vec) {
    return vec * scalar;
}

// ── Vector3 Algorithms ───────────────────────────────────────────────────────
template<typename T>
constexpr T Dot(const Vector3<T>& a, const Vector3<T>& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename T>
constexpr Vector3<T> Cross(const Vector3<T>& a, const Vector3<T>& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

template<typename T>
constexpr T LengthSquared(const Vector3<T>& vec) {
    return Dot(vec, vec);
}

template<typename T>
inline T Length(const Vector3<T>& vec) {
    return Sqrt(LengthSquared(vec));
}

template<typename T>
inline T DistanceSquared(const Vector3<T>& a, const Vector3<T>& b) {
    return LengthSquared(a - b);
}

template<typename T>
inline T Distance(const Vector3<T>& a, const Vector3<T>& b) {
    return Length(a - b);
}

template<typename T>
inline Vector3<T> Normalize(const Vector3<T>& vec) {
    T len = Length(vec);
    return vec / len;
}

template<typename T>
inline Vector3<T> SafeNormalize(const Vector3<T>& vec, T epsilon = static_cast<T>(1e-6)) {
    T len = Length(vec);
    if (len <= epsilon) {
        return Vector3<T>::Zero();
    }
    return vec / len;
}

template<typename T>
constexpr Vector3<T> Project(const Vector3<T>& a, const Vector3<T>& b) {
    T bLenSq = LengthSquared(b);
    if (bLenSq == 0) return Vector3<T>::Zero();
    return b * (Dot(a, b) / bLenSq);
}

template<typename T>
constexpr Vector3<T> Reject(const Vector3<T>& a, const Vector3<T>& b) {
    return a - Project(a, b);
}

template<typename T>
constexpr Vector3<T> Reflect(const Vector3<T>& vec, const Vector3<T>& normal) {
    return vec - normal * static_cast<T>(2.0) * Dot(vec, normal);
}

template<typename T>
inline Vector3<T> Refract(const Vector3<T>& incident, const Vector3<T>& normal, T eta) {
    T dotNI = Dot(normal, incident);
    T k = static_cast<T>(1.0) - eta * eta * (static_cast<T>(1.0) - dotNI * dotNI);
    if (k < 0.0) {
        return Vector3<T>::Zero();
    }
    return incident * eta - normal * (eta * dotNI + Sqrt(k));
}


template<typename T>
inline T AngleBetween(const Vector3<T>& a, const Vector3<T>& b) {
    T lenA = Length(a);
    T lenB = Length(b);
    if (lenA == 0 || lenB == 0) return 0;
    return std::acos(std::clamp(Dot(a, b) / (lenA * lenB), static_cast<T>(-1.0), static_cast<T>(1.0)));
}

template<typename T>
constexpr Vector3<T> Clamp(const Vector3<T>& vec, const Vector3<T>& minVal, const Vector3<T>& maxVal) {
    return {
        std::clamp(vec.x, minVal.x, maxVal.x),
        std::clamp(vec.y, minVal.y, maxVal.y),
        std::clamp(vec.z, minVal.z, maxVal.z)
    };
}

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;
using Vector3i = Vector3<i32>;

} // namespace Shape::Math

// Custom Hash Specialization
namespace std {
template<typename T>
struct hash<Shape::Math::Vector3<T>> {
    size_t operator()(const Shape::Math::Vector3<T>& vec) const {
        size_t h1 = hash<T>{}(vec.x);
        size_t h2 = hash<T>{}(vec.y);
        size_t h3 = hash<T>{}(vec.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
}
