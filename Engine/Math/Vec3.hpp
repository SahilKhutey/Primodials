#pragma once

#include "Core/Platform.hpp"
#include "Math/Vec2.hpp"
#include <cmath>
#include <ostream>

namespace Shape::Math {

// ─────────────────────────────────────────────────────────────────────────────
//  Vec3<T> — 3-component vector
// ─────────────────────────────────────────────────────────────────────────────
template<typename T>
struct Vec3 {
    T x{}, y{}, z{};

    constexpr Vec3() = default;
    constexpr Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
    explicit constexpr Vec3(T scalar) : x(scalar), y(scalar), z(scalar) {}
    constexpr Vec3(Vec2<T> xy, T z_) : x(xy.x), y(xy.y), z(z_) {}

    static constexpr Vec3 Zero()    { return { T(0), T(0), T(0) }; }
    static constexpr Vec3 One()     { return { T(1), T(1), T(1) }; }
    static constexpr Vec3 UnitX()   { return { T(1), T(0), T(0) }; }
    static constexpr Vec3 UnitY()   { return { T(0), T(1), T(0) }; }
    static constexpr Vec3 UnitZ()   { return { T(0), T(0), T(1) }; }
    static constexpr Vec3 Up()      { return { T(0), T(1), T(0) }; }
    static constexpr Vec3 Forward() { return { T(0), T(0), T(-1) }; }
    static constexpr Vec3 Right()   { return { T(1), T(0), T(0) }; }

    constexpr Vec3 operator+(const Vec3& o) const { return { x+o.x, y+o.y, z+o.z }; }
    constexpr Vec3 operator-(const Vec3& o) const { return { x-o.x, y-o.y, z-o.z }; }
    constexpr Vec3 operator*(const Vec3& o) const { return { x*o.x, y*o.y, z*o.z }; }
    constexpr Vec3 operator/(const Vec3& o) const { return { x/o.x, y/o.y, z/o.z }; }

    constexpr Vec3 operator+(T s) const { return { x+s, y+s, z+s }; }
    constexpr Vec3 operator-(T s) const { return { x-s, y-s, z-s }; }
    constexpr Vec3 operator*(T s) const { return { x*s, y*s, z*s }; }
    constexpr Vec3 operator/(T s) const { return { x/s, y/s, z/s }; }
    constexpr Vec3 operator-()    const { return { -x, -y, -z }; }

    Vec3& operator+=(const Vec3& o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
    Vec3& operator-=(const Vec3& o) { x-=o.x; y-=o.y; z-=o.z; return *this; }
    Vec3& operator*=(const Vec3& o) { x*=o.x; y*=o.y; z*=o.z; return *this; }
    Vec3& operator/=(const Vec3& o) { x/=o.x; y/=o.y; z/=o.z; return *this; }
    Vec3& operator*=(T s)           { x*=s;   y*=s;   z*=s;   return *this; }
    Vec3& operator/=(T s)           { x/=s;   y/=s;   z/=s;   return *this; }

    constexpr bool operator==(const Vec3& o) const { return x==o.x && y==o.y && z==o.z; }
    constexpr bool operator!=(const Vec3& o) const { return !(*this == o); }

    T&       operator[](int i)       { return (&x)[i]; }
    const T& operator[](int i) const { return (&x)[i]; }

    constexpr T   Dot(const Vec3& o) const { return x*o.x + y*o.y + z*o.z; }
    constexpr Vec3 Cross(const Vec3& o) const {
        return { y*o.z - z*o.y,
                 z*o.x - x*o.z,
                 x*o.y - y*o.x };
    }
    constexpr T   LengthSquared() const { return x*x + y*y + z*z; }
    T             Length() const { return static_cast<T>(std::sqrt(static_cast<f64>(LengthSquared()))); }

    Vec3 Normalized() const {
        T len = Length();
        return (len > T(1e-10)) ? Vec3{ x/len, y/len, z/len } : Zero();
    }
    void Normalize() { *this = Normalized(); }

    T DistanceTo(const Vec3& o) const { return (*this - o).Length(); }
    T DistanceSquaredTo(const Vec3& o) const { return (*this - o).LengthSquared(); }

    Vec3 Lerp(const Vec3& to, f32 t) const {
        return { x+(to.x-x)*T(t), y+(to.y-y)*T(t), z+(to.z-z)*T(t) };
    }

    Vec2<T> XY() const { return { x, y }; }
    Vec2<T> XZ() const { return { x, z }; }

    // Reflect across normal n (n must be unit)
    Vec3 Reflect(const Vec3& n) const { return *this - n * (T(2) * Dot(n)); }

    friend std::ostream& operator<<(std::ostream& os, const Vec3& v) {
        return os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
    }
};

template<typename T> constexpr Vec3<T> operator*(T s, const Vec3<T>& v) { return v * s; }
template<typename T> constexpr Vec3<T> operator+(T s, const Vec3<T>& v) { return v + s; }

using Vec3f = Vec3<f32>;
using Vec3d = Vec3<f64>;
using Vec3i = Vec3<i32>;

} // namespace Shape::Math

namespace Shape {
    using Math::Vec3f;
    using Math::Vec3i;
}
