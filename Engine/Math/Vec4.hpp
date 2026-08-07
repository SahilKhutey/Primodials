#pragma once

#include "Core/Platform.hpp"
#include "Math/Vec3.hpp"
#include <ostream>

namespace Shape::Math {

// ─────────────────────────────────────────────────────────────────────────────
//  Vec4<T> — 4-component vector  (also used as RGBA float colour)
// ─────────────────────────────────────────────────────────────────────────────
template<typename T>
struct Vec4 {
    T x{}, y{}, z{}, w{};

    constexpr Vec4() = default;
    constexpr Vec4(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) {}
    explicit constexpr Vec4(T s) : x(s), y(s), z(s), w(s) {}
    constexpr Vec4(Vec3<T> xyz, T w_) : x(xyz.x), y(xyz.y), z(xyz.z), w(w_) {}
    constexpr Vec4(Vec2<T> xy, T z_, T w_) : x(xy.x), y(xy.y), z(z_), w(w_) {}

    static constexpr Vec4 Zero() { return { T(0), T(0), T(0), T(0) }; }
    static constexpr Vec4 One()  { return { T(1), T(1), T(1), T(1) }; }

    // Arithmetic
    constexpr Vec4 operator+(const Vec4& o) const { return { x+o.x, y+o.y, z+o.z, w+o.w }; }
    constexpr Vec4 operator-(const Vec4& o) const { return { x-o.x, y-o.y, z-o.z, w-o.w }; }
    constexpr Vec4 operator*(const Vec4& o) const { return { x*o.x, y*o.y, z*o.z, w*o.w }; }
    constexpr Vec4 operator/(const Vec4& o) const { return { x/o.x, y/o.y, z/o.z, w/o.w }; }
    constexpr Vec4 operator*(T s) const { return { x*s, y*s, z*s, w*s }; }
    constexpr Vec4 operator/(T s) const { return { x/s, y/s, z/s, w/s }; }
    constexpr Vec4 operator-()    const { return { -x, -y, -z, -w }; }

    Vec4& operator+=(const Vec4& o) { x+=o.x; y+=o.y; z+=o.z; w+=o.w; return *this; }
    Vec4& operator-=(const Vec4& o) { x-=o.x; y-=o.y; z-=o.z; w-=o.w; return *this; }
    Vec4& operator*=(T s)           { x*=s;   y*=s;   z*=s;   w*=s;   return *this; }
    Vec4& operator/=(T s)           { x/=s;   y/=s;   z/=s;   w/=s;   return *this; }

    constexpr bool operator==(const Vec4& o) const { return x==o.x&&y==o.y&&z==o.z&&w==o.w; }
    constexpr bool operator!=(const Vec4& o) const { return !(*this==o); }

    T&       operator[](int i)       { return (&x)[i]; }
    const T& operator[](int i) const { return (&x)[i]; }

    // Geometry
    constexpr T    Dot(const Vec4& o) const { return x*o.x + y*o.y + z*o.z + w*o.w; }
    constexpr T    LengthSquared()    const { return x*x + y*y + z*z + w*w; }
    T              Length()           const { return static_cast<T>(std::sqrt(static_cast<f64>(LengthSquared()))); }

    Vec4 Normalized() const {
        T len = Length();
        return (len > T(1e-10)) ? Vec4{ x/len, y/len, z/len, w/len } : Zero();
    }

    // Swizzles
    Vec3<T> XYZ() const { return { x, y, z }; }
    Vec2<T> XY()  const { return { x, y }; }

    // Homogeneous divide
    Vec3<T> PerspectiveDivide() const {
        T inv = T(1) / w;
        return { x*inv, y*inv, z*inv };
    }

    Vec4 Lerp(const Vec4& to, f32 t) const {
        return { x+(to.x-x)*T(t), y+(to.y-y)*T(t),
                 z+(to.z-z)*T(t), w+(to.w-w)*T(t) };
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec4& v) {
        return os << "Vec4(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
    }
};

template<typename T> constexpr Vec4<T> operator*(T s, const Vec4<T>& v) { return v * s; }

using Vec4f = Vec4<f32>;
using Vec4d = Vec4<f64>;
using Vec4i = Vec4<i32>;

} // namespace Shape::Math

namespace Shape {
    using Math::Vec4f;
    using Math::Vec4i;
}
