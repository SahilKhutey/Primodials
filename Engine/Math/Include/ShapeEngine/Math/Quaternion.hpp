#pragma once

#include "ShapeEngine/Math/Vector3.hpp"
#include "ShapeEngine/Math/Angle.hpp"
#include <cmath>
#include <optional>

namespace Shape::Math {

template<typename T>
struct Quaternion {
    T x = 0;
    T y = 0;
    T z = 0;
    T w = 1;

    // ── Constructors ──────────────────────────────────────────────────────────
    constexpr Quaternion() : x(0), y(0), z(0), w(1) {}
    constexpr Quaternion(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}

    // ── Static Constructors ───────────────────────────────────────────────────
    static constexpr Quaternion Identity() {
        return { 0, 0, 0, 1 };
    }

    static constexpr Quaternion Zero() {
        return { 0, 0, 0, 0 };
    }

    static Quaternion AxisAngle(const Vector3<T>& axis, Radians angle) {
        Vector3<T> n = SafeNormalize(axis);
        T half = static_cast<T>(angle.value) * static_cast<T>(0.5);
        T s    = static_cast<T>(std::sin(static_cast<double>(half)));
        T c    = static_cast<T>(std::cos(static_cast<double>(half)));
        return { n.x * s, n.y * s, n.z * s, c };
    }

    static Quaternion EulerXYZ(Radians pitch, Radians yaw, Radians roll) {
        Quaternion qx = AxisAngle(Vector3<T>::UnitX(), pitch);
        Quaternion qy = AxisAngle(Vector3<T>::UnitY(), yaw);
        Quaternion qz = AxisAngle(Vector3<T>::UnitZ(), roll);
        return qz * qy * qx;
    }

    static Quaternion EulerZYX(Radians roll, Radians yaw, Radians pitch) {
        Quaternion qx = AxisAngle(Vector3<T>::UnitX(), pitch);
        Quaternion qy = AxisAngle(Vector3<T>::UnitY(), yaw);
        Quaternion qz = AxisAngle(Vector3<T>::UnitZ(), roll);
        return qx * qy * qz;
    }

    static Quaternion LookRotation(const Vector3<T>& forward, const Vector3<T>& up) {
        Vector3<T> f = SafeNormalize(forward);
        Vector3<T> r = SafeNormalize(Cross(up, f));
        Vector3<T> u = Cross(f, r);

        T m00 = r.x; T m01 = r.y; T m02 = r.z;
        T m10 = u.x; T m11 = u.y; T m12 = u.z;
        T m20 = f.x; T m21 = f.y; T m22 = f.z;

        T trace = m00 + m11 + m22;
        Quaternion q;

        if (trace > 0) {
            T s = static_cast<T>(0.5) / static_cast<T>(std::sqrt(static_cast<double>(trace + 1)));
            q.w = static_cast<T>(0.25) / s;
            q.x = (m12 - m21) * s;
            q.y = (m20 - m02) * s;
            q.z = (m01 - m10) * s;
        } else if (m00 > m11 && m00 > m22) {
            T s = static_cast<T>(2) * static_cast<T>(std::sqrt(static_cast<double>(1 + m00 - m11 - m22)));
            q.w = (m12 - m21) / s;
            q.x = static_cast<T>(0.25) * s;
            q.y = (m01 + m10) / s;
            q.z = (m20 + m02) / s;
        } else if (m11 > m22) {
            T s = static_cast<T>(2) * static_cast<T>(std::sqrt(static_cast<double>(1 + m11 - m00 - m22)));
            q.w = (m20 - m02) / s;
            q.x = (m01 + m10) / s;
            q.y = static_cast<T>(0.25) * s;
            q.z = (m12 + m21) / s;
        } else {
            T s = static_cast<T>(2) * static_cast<T>(std::sqrt(static_cast<double>(1 + m22 - m00 - m11)));
            q.w = (m01 - m10) / s;
            q.x = (m20 + m02) / s;
            q.y = (m12 + m21) / s;
            q.z = static_cast<T>(0.25) * s;
        }
        return q;
    }

    static Quaternion FromToRotation(const Vector3<T>& from, const Vector3<T>& to) {
        Vector3<T> f   = SafeNormalize(from);
        Vector3<T> to_n = SafeNormalize(to);
        T cosAngle = Math::Dot(f, to_n);

        if (cosAngle >= static_cast<T>(1) - static_cast<T>(1e-6)) {
            return Identity();
        }
        if (cosAngle <= static_cast<T>(-1) + static_cast<T>(1e-6)) {
            Vector3<T> perp = Math::Cross(f, Vector3<T>::UnitX());
            if (Math::LengthSquared(perp) < static_cast<T>(1e-6)) {
                perp = Math::Cross(f, Vector3<T>::UnitY());
            }
            return AxisAngle(SafeNormalize(perp), Radians(static_cast<f32>(PI)));
        }
        Vector3<T> axis = Math::Cross(f, to_n);
        T halfCos = static_cast<T>(std::sqrt(static_cast<double>((1 + cosAngle) * 0.5)));
        T halfSin = static_cast<T>(std::sqrt(static_cast<double>((1 - cosAngle) * 0.5)));
        axis = SafeNormalize(axis) * halfSin;
        return { axis.x, axis.y, axis.z, halfCos };
    }

    // ── Operators ─────────────────────────────────────────────────────────────
    constexpr bool operator==(const Quaternion& o) const {
        return x == o.x && y == o.y && z == o.z && w == o.w;
    }
    constexpr bool operator!=(const Quaternion& o) const { return !(*this == o); }

    constexpr Quaternion operator+(const Quaternion& o) const {
        return { x + o.x, y + o.y, z + o.z, w + o.w };
    }
    constexpr Quaternion operator-(const Quaternion& o) const {
        return { x - o.x, y - o.y, z - o.z, w - o.w };
    }
    constexpr Quaternion operator*(T scalar) const {
        return { x * scalar, y * scalar, z * scalar, w * scalar };
    }

    // Quaternion multiplication — composition order: (this) then (o)
    constexpr Quaternion operator*(const Quaternion& o) const {
        return {
            w * o.x + x * o.w + y * o.z - z * o.y,
            w * o.y - x * o.z + y * o.w + z * o.x,
            w * o.z + x * o.y - y * o.x + z * o.w,
            w * o.w - x * o.x - y * o.y - z * o.z
        };
    }
    Quaternion& operator*=(const Quaternion& o) { *this = *this * o; return *this; }

    constexpr Quaternion operator-() const { return { -x, -y, -z, -w }; }

    // ── Core Operations ───────────────────────────────────────────────────────
    T LengthSquared() const { return x * x + y * y + z * z + w * w; }
    T Length() const { return static_cast<T>(std::sqrt(static_cast<double>(LengthSquared()))); }

    T Dot(const Quaternion& o) const { return x * o.x + y * o.y + z * o.z + w * o.w; }

    Quaternion Normalized() const {
        T len = Length();
        if (len > static_cast<T>(1e-10)) {
            return { x / len, y / len, z / len, w / len };
        }
        return Identity();
    }
    void Normalize() { *this = Normalized(); }

    void NormalizeIfNeeded(T threshold = static_cast<T>(1e-3)) {
        T lenSq = LengthSquared();
        if (std::abs(static_cast<double>(lenSq - 1)) > static_cast<double>(threshold)) {
            Normalize();
        }
    }

    constexpr Quaternion Conjugate() const { return { -x, -y, -z, w }; }

    Quaternion Inverse() const {
        T lenSq = LengthSquared();
        return Conjugate() * (static_cast<T>(1) / lenSq);
    }

    std::optional<Quaternion> SafeInverse(T epsilon = static_cast<T>(1e-10)) const {
        T lenSq = LengthSquared();
        if (lenSq < epsilon) return std::nullopt;
        return Conjugate() * (static_cast<T>(1) / lenSq);
    }

    // ── Rotation Functions ────────────────────────────────────────────────────
    Vector3<T> Rotate(const Vector3<T>& v) const {
        Vector3<T> u{ x, y, z };
        return u * (static_cast<T>(2) * Math::Dot(u, v))
             + v  * (w * w - Math::Dot(u, u))
             + Math::Cross(u, v) * (static_cast<T>(2) * w);
    }

    // ── Conversion Functions ──────────────────────────────────────────────────
    Vector3<T> ToEuler() const {
        T sinrCosp = static_cast<T>(2) * (w * x + y * z);
        T cosrCosp = static_cast<T>(1) - static_cast<T>(2) * (x * x + y * y);
        T pitch = static_cast<T>(std::atan2(static_cast<double>(sinrCosp), static_cast<double>(cosrCosp)));

        T sinp = static_cast<T>(2) * (w * y - z * x);
        T yaw;
        if (std::abs(static_cast<double>(sinp)) >= 1.0) {
            yaw = static_cast<T>(std::copysign(PI * 0.5, static_cast<double>(sinp)));
        } else {
            yaw = static_cast<T>(std::asin(static_cast<double>(sinp)));
        }

        T sinyCosp = static_cast<T>(2) * (w * z + x * y);
        T cosyCosp = static_cast<T>(1) - static_cast<T>(2) * (y * y + z * z);
        T roll = static_cast<T>(std::atan2(static_cast<double>(sinyCosp), static_cast<double>(cosyCosp)));

        return { pitch, yaw, roll };
    }

    // ── Interpolation ─────────────────────────────────────────────────────────
    static Quaternion Lerp(const Quaternion& a, const Quaternion& b, T t) {
        return (a + (b - a) * t);
    }

    static Quaternion NLerp(const Quaternion& a, const Quaternion& b, T t) {
        return Lerp(a, b, t).Normalized();
    }

    static Quaternion SLerp(Quaternion a, Quaternion b, T t) {
        T dot = a.Dot(b);
        if (dot < 0) { b = -b; dot = -dot; }

        if (dot > static_cast<T>(0.9995)) {
            return NLerp(a, b, t);
        }

        T theta0   = static_cast<T>(std::acos(static_cast<double>(dot)));
        T theta    = theta0 * t;
        T sinTheta = static_cast<T>(std::sin(static_cast<double>(theta)));
        T sinTheta0 = static_cast<T>(std::sin(static_cast<double>(theta0)));

        T sa = sinTheta / sinTheta0;
        T sb = static_cast<T>(std::cos(static_cast<double>(theta))) - dot * sa;
        return { a.x * sb + b.x * sa, a.y * sb + b.y * sa,
                 a.z * sb + b.z * sa, a.w * sb + b.w * sa };
    }

    // ── Advanced Operations ───────────────────────────────────────────────────
    Quaternion Log() const {
        T lenVec = static_cast<T>(std::sqrt(static_cast<double>(x*x + y*y + z*z)));
        if (lenVec < static_cast<T>(1e-10)) {
            return { 0, 0, 0, static_cast<T>(std::log(static_cast<double>(w))) };
        }
        T theta = static_cast<T>(std::atan2(static_cast<double>(lenVec), static_cast<double>(w)));
        T scale = theta / lenVec;
        return { x * scale, y * scale, z * scale, static_cast<T>(std::log(static_cast<double>(Length()))) };
    }

    Quaternion Exp() const {
        T lenVec = static_cast<T>(std::sqrt(static_cast<double>(x*x + y*y + z*z)));
        T expW   = static_cast<T>(std::exp(static_cast<double>(w)));
        if (lenVec < static_cast<T>(1e-10)) {
            return { 0, 0, 0, expW };
        }
        T scale = expW * static_cast<T>(std::sin(static_cast<double>(lenVec))) / lenVec;
        return { x * scale, y * scale, z * scale, expW * static_cast<T>(std::cos(static_cast<double>(lenVec))) };
    }

    Quaternion Pow(T t) const {
        return (Log() * t).Exp();
    }

    static Quaternion Difference(const Quaternion& from, const Quaternion& to) {
        return from.Inverse() * to;
    }

    // ── Angular Velocity Integration ──────────────────────────────────────────
    // Integrate angular velocity (rad/s) over dt seconds
    Quaternion IntegrateAngularVelocity(const Vector3<T>& omega, T dt) const {
        T speed = Math::Length(omega);
        T angle = speed * dt;
        if (angle < static_cast<T>(1e-10)) return *this;
        Vector3<T> axis = SafeNormalize(omega);
        Quaternion dq = AxisAngle(axis, Radians(static_cast<f32>(angle)));
        return (*this * dq).Normalized();
    }

    // ── Numerical Stability ───────────────────────────────────────────────────
    bool NearIdentity(T epsilon = static_cast<T>(1e-6)) const {
        return NearEquals(x, static_cast<T>(0), epsilon) &&
               NearEquals(y, static_cast<T>(0), epsilon) &&
               NearEquals(z, static_cast<T>(0), epsilon) &&
               NearEquals(w, static_cast<T>(1), epsilon);
    }

    bool NearZero(T epsilon = static_cast<T>(1e-6)) const {
        return LengthSquared() < epsilon;
    }
};

// Type aliases
using Quaternionf = Quaternion<float>;
using Quaterniond = Quaternion<double>;

} // namespace Shape::Math
