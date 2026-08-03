#pragma once

#include "Core/Platform.hpp"
#include <cmath>

namespace Shape::Math {

constexpr f32 PI = 3.14159265358979323846f;
constexpr f32 HALF_PI = 1.57079632679489661923f;
constexpr f32 TWO_PI = 6.28318530717958647692f;
constexpr f32 EPSILON = 1e-6f;
constexpr f32 DEG_TO_RAD = PI / 180.0f;
constexpr f32 RAD_TO_DEG = 180.0f / PI;

template <typename T>
SHAPE_INLINE constexpr T Min(T a, T b) {
    return (a < b) ? a : b;
}

template <typename T>
SHAPE_INLINE constexpr T Max(T a, T b) {
    return (a > b) ? a : b;
}

template <typename T>
SHAPE_INLINE constexpr T Clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

template <typename T>
SHAPE_INLINE constexpr T Abs(T value) {
    return (value < 0) ? -value : value;
}

template <typename T>
SHAPE_INLINE constexpr T Sign(T value) {
    return (value < 0) ? static_cast<T>(-1) : static_cast<T>(1);
}

template <typename T>
SHAPE_INLINE constexpr T Lerp(T a, T b, f32 t) {
    return a + static_cast<T>((b - a) * t);
}

SHAPE_INLINE bool NearEquals(f32 a, f32 b, f32 epsilon = EPSILON) {
    return Abs(a - b) <= epsilon;
}

SHAPE_INLINE f32 Sqrt(f32 value) {
    return std::sqrt(value);
}

SHAPE_INLINE f32 Sin(f32 radians) {
    return std::sin(radians);
}

SHAPE_INLINE f32 Cos(f32 radians) {
    return std::cos(radians);
}

SHAPE_INLINE f32 Tan(f32 radians) {
    return std::tan(radians);
}

SHAPE_INLINE f32 ACos(f32 value) {
    return std::acos(value);
}

SHAPE_INLINE f32 ASin(f32 value) {
    return std::asin(value);
}

SHAPE_INLINE f32 ATan2(f32 y, f32 x) {
    return std::atan2(y, x);
}

SHAPE_INLINE f32 Floor(f32 value) { return std::floor(value); }
SHAPE_INLINE f32 Ceil(f32 value)  { return std::ceil(value);  }
SHAPE_INLINE f32 Round(f32 value) { return std::round(value); }
SHAPE_INLINE f32 Pow(f32 base, f32 exp) { return std::pow(base, exp); }
SHAPE_INLINE f32 Log(f32 value)   { return std::log(value); }
SHAPE_INLINE f32 Log2(f32 value)  { return std::log2(value); }
SHAPE_INLINE f32 Exp(f32 value)   { return std::exp(value); }
SHAPE_INLINE f32 Mod(f32 x, f32 y) { return std::fmod(x, y); }

/// Smooth Hermite interpolation — no overshooting, derivative is 0 at edges.
SHAPE_INLINE f32 SmoothStep(f32 edge0, f32 edge1, f32 x) {
    f32 t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

/// Wrap value into [0, max).
SHAPE_INLINE f32 Wrap(f32 value, f32 max) {
    if (max <= 0.0f) return 0.0f;
    f32 r = Mod(value, max);
    return (r < 0.0f) ? r + max : r;
}

/// Wrap value into [min, max).
SHAPE_INLINE f32 WrapRange(f32 value, f32 min_, f32 max_) {
    return min_ + Wrap(value - min_, max_ - min_);
}

SHAPE_INLINE bool NearlyZero(f32 v, f32 eps = EPSILON) { return Abs(v) <= eps; }

/// Remap a value from [inLow,inHigh] to [outLow,outHigh].
template<typename T>
SHAPE_INLINE constexpr T Remap(T value, T inLow, T inHigh, T outLow, T outHigh) {
    return outLow + (value - inLow) * (outHigh - outLow) / (inHigh - inLow);
}

template<typename T>
SHAPE_INLINE constexpr T Square(T value) {
    return value * value;
}

template<typename T>
SHAPE_INLINE constexpr T Cube(T value) {
    return value * value * value;
}

} // namespace Shape::Math
