#pragma once

#include "Core/Platform.hpp"
#include "Math/MathUtils.hpp"

namespace Shape::Math {

struct Degrees;

struct Radians {
    f32 value = 0.0f;

    constexpr Radians() = default;
    constexpr explicit Radians(f32 val) : value(val) {}
    constexpr inline Radians(const Degrees& deg);

    constexpr Radians  operator-()               const { return Radians(-value); }
    constexpr Radians  operator+(Radians o)      const { return Radians(value + o.value); }
    constexpr Radians  operator-(Radians o)      const { return Radians(value - o.value); }
    constexpr Radians  operator*(f32 s)          const { return Radians(value * s); }
    constexpr operator f32()                     const { return value; }
};

struct Degrees {
    f32 value = 0.0f;

    constexpr Degrees() = default;
    constexpr explicit Degrees(f32 val) : value(val) {}
    constexpr inline Degrees(const Radians& rad);

    constexpr Degrees  operator-()               const { return Degrees(-value); }
    constexpr Degrees  operator+(Degrees o)      const { return Degrees(value + o.value); }
    constexpr Degrees  operator-(Degrees o)      const { return Degrees(value - o.value); }
    constexpr Degrees  operator*(f32 s)          const { return Degrees(value * s); }
    constexpr operator f32()                     const { return value; }
};

// Out-of-line conversions (both types fully defined)
constexpr Radians::Radians(const Degrees& deg) : value(deg.value * DEG_TO_RAD) {}
constexpr Degrees::Degrees(const Radians& rad) : value(rad.value * RAD_TO_DEG) {}

// User-defined literals
inline namespace Literals {
    constexpr Radians operator""_rad(long double val) {
        return Radians(static_cast<f32>(val));
    }
    constexpr Radians operator""_rad(unsigned long long val) {
        return Radians(static_cast<f32>(val));
    }

    constexpr Degrees operator""_deg(long double val) {
        return Degrees(static_cast<f32>(val));
    }
    constexpr Degrees operator""_deg(unsigned long long val) {
        return Degrees(static_cast<f32>(val));
    }
} // namespace Literals

} // namespace Shape::Math
