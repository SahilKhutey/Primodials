#pragma once

#include "Core/Platform.hpp"
#include "Math/Vec3.hpp"
#include "Math/Vec4.hpp"
#include <cmath>

namespace Shape::Math {

// ─────────────────────────────────────────────────────────────────────────────
//  Quat<T> — unit quaternion for 3-D rotation.
//  Storage: (x, y, z, w)  where w is the scalar part.
//  Convention: left-hand multiplication  (q1 * q2 applies q1 FIRST, then q2).
// ─────────────────────────────────────────────────────────────────────────────
template<typename T>
struct Quat {
    T x{}, y{}, z{}, w{ T(1) };

    constexpr Quat() = default;
    constexpr Quat(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) {}

    // Identity rotation
    static constexpr Quat Identity() { return { T(0), T(0), T(0), T(1) }; }

    // Construct from axis-angle (axis need not be normalised)
    static Quat FromAxisAngle(const Vec3<T>& axis, T angleRad) {
        Vec3<T> n = axis.Normalized();
        T half  = angleRad * T(0.5);
        T s     = static_cast<T>(std::sin(static_cast<f64>(half)));
        T c     = static_cast<T>(std::cos(static_cast<f64>(half)));
        return { n.x * s, n.y * s, n.z * s, c };
    }

    // Construct from Euler angles (pitch=X, yaw=Y, roll=Z), applied X→Y→Z
    static Quat FromEuler(T pitch, T yaw, T roll) {
        Quat qx = FromAxisAngle(Vec3<T>::UnitX(), pitch);
        Quat qy = FromAxisAngle(Vec3<T>::UnitY(), yaw);
        Quat qz = FromAxisAngle(Vec3<T>::UnitZ(), roll);
        return qz * qy * qx;
    }

    // Quaternion multiplication (compose rotations)
    Quat operator*(const Quat& o) const {
        return {
            w*o.x + x*o.w + y*o.z - z*o.y,
            w*o.y - x*o.z + y*o.w + z*o.x,
            w*o.z + x*o.y - y*o.x + z*o.w,
            w*o.w - x*o.x - y*o.y - z*o.z
        };
    }
    Quat& operator*=(const Quat& o) { *this = *this * o; return *this; }

    constexpr bool operator==(const Quat& o) const {
        return x==o.x && y==o.y && z==o.z && w==o.w;
    }
    constexpr bool operator!=(const Quat& o) const { return !(*this==o); }

    // Magnitude
    T LengthSquared() const { return x*x + y*y + z*z + w*w; }
    T Length()        const { return static_cast<T>(std::sqrt(static_cast<f64>(LengthSquared()))); }

    Quat Normalized() const {
        T len = Length();
        return (len > T(1e-10)) ? Quat{ x/len, y/len, z/len, w/len } : Identity();
    }
    void Normalize() { *this = Normalized(); }

    // Conjugate (inverse for unit quaternions)
    constexpr Quat Conjugate() const { return { -x, -y, -z, w }; }
    Quat Inverse() const { return Conjugate().Normalized(); }

    // Rotate a point
    Vec3<T> Rotate(const Vec3<T>& v) const {
        Vec3<T> u{ x, y, z };
        return u * (T(2) * u.Dot(v))
             + v  * (w*w - u.Dot(u))
             + u.Cross(v) * (T(2) * w);
    }

    // Spherical linear interpolation
    static Quat Slerp(Quat a, Quat b, T t) {
        T dot = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
        if (dot < T(0)) { b = { -b.x, -b.y, -b.z, -b.w }; dot = -dot; }
        if (dot > T(0.9995)) {
            // Linear interpolation for nearly identical quaternions
            Quat r { a.x+(b.x-a.x)*t, a.y+(b.y-a.y)*t,
                     a.z+(b.z-a.z)*t, a.w+(b.w-a.w)*t };
            return r.Normalized();
        }
        T theta0    = static_cast<T>(std::acos(static_cast<f64>(dot)));
        T theta     = theta0 * t;
        T sinTheta  = static_cast<T>(std::sin(static_cast<f64>(theta)));
        T sinTheta0 = static_cast<T>(std::sin(static_cast<f64>(theta0)));
        T sa = sinTheta / sinTheta0;
        T sb = static_cast<T>(std::cos(static_cast<f64>(theta))) - dot * sa;
        return { a.x*sb + b.x*sa, a.y*sb + b.y*sa,
                 a.z*sb + b.z*sa, a.w*sb + b.w*sa };
    }

    // Extract Euler angles (radians) — order: pitch(X), yaw(Y), roll(Z)
    Vec3<T> ToEuler() const {
        T sinrCosp =  T(2) * (w*x + y*z);
        T cosrCosp =  T(1) - T(2) * (x*x + y*y);
        T pitch = static_cast<T>(std::atan2(static_cast<f64>(sinrCosp), static_cast<f64>(cosrCosp)));

        T sinp = T(2) * (w*y - z*x);
        T yaw;
        if (std::abs(static_cast<f64>(sinp)) >= 1.0)
            yaw = static_cast<T>(std::copysign(3.14159265 * 0.5, static_cast<f64>(sinp)));
        else
            yaw = static_cast<T>(std::asin(static_cast<f64>(sinp)));

        T sinyCosp =  T(2) * (w*z + x*y);
        T cosyCosp =  T(1) - T(2) * (y*y + z*z);
        T roll = static_cast<T>(std::atan2(static_cast<f64>(sinyCosp), static_cast<f64>(cosyCosp)));

        return { pitch, yaw, roll };
    }

    // 2-D rotation angle (around Z axis)
    T ToAngle2D() const { return T(2) * static_cast<T>(std::atan2(static_cast<f64>(z), static_cast<f64>(w))); }

    // Vec4 representation for shader upload
    Vec4<T> AsVec4() const { return { x, y, z, w }; }
};

using Quatf = Quat<f32>;
using Quatd = Quat<f64>;

} // namespace Shape::Math

namespace Shape {
    using Math::Quatf;
}
