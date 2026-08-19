/**
 * Primordials — SIMD Math Utilities (C++23)
 *
 * Vector operations and geometric algorithms.
 *
 * License: MIT
 */

#pragma once

#include <cmath>
#include <concepts>

namespace primordials::math {

constexpr float PI = 3.14159265358979323846f;
constexpr float TAU = 6.28318530717958647692f;
constexpr float EPSILON = 1e-5f;

template<std::floating_point T>
[[nodiscard]] constexpr T lerp(T a, T b, T t) noexcept {
    return a + (b - a) * t;
}

template<std::floating_point T>
[[nodiscard]] constexpr T clamp(T value, T min, T max) noexcept {
    return value < min ? min : (value > max ? max : value);
}

struct Vec2 {
    float x;
    float y;

    constexpr Vec2() noexcept : x(0.0f), y(0.0f) {}
    constexpr Vec2(float x, float y) noexcept : x(x), y(y) {}

    constexpr Vec2 operator+(const Vec2& o) const noexcept { return {x + o.x, y + o.y}; }
    constexpr Vec2 operator-(const Vec2& o) const noexcept { return {x - o.x, y - o.y}; }
    constexpr Vec2 operator*(float s) const noexcept { return {x * s, y * s}; }

    [[nodiscard]] constexpr float length_sq() const noexcept { return x * x + y * y; }
    [[nodiscard]] float length() const noexcept { return std::sqrt(length_sq()); }
    [[nodiscard]] Vec2 normalized() const noexcept {
        const float len = length();
        return len > EPSILON ? Vec2{x / len, y / len} : Vec2{};
    }
};

} // namespace primordials::math
