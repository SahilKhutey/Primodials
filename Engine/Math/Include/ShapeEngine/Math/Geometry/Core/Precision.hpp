#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  Precision.hpp
//  Precision policy system for geometry computations.
//
//  Selects between float, double, long double, or future fixed-point
//  representations without changing the algorithm code.
//
//  Usage:
//      using MyPrecision = DoublePrecision;
//      Point3<MyPrecision::ScalarType> pt;
// ─────────────────────────────────────────────────────────────────────────────

#include "Core/Platform.hpp"
#include <limits>
#include <cmath>

namespace Shape::Math::Geometry {

// ── Precision Tag Types ───────────────────────────────────────────────────────

struct SinglePrecision {
    using ScalarType     = f32;
    using HighScalarType = f64;

    static constexpr f32 Epsilon    = 1e-6f;
    static constexpr f32 SqrtEpsilon = 1e-3f;
    static constexpr f32 SmallEps  = 1e-10f;

    static constexpr const char* Name = "float";
};

struct DoublePrecision {
    using ScalarType     = f64;
    using HighScalarType = f64;

    static constexpr f64 Epsilon    = 1e-12;
    static constexpr f64 SqrtEpsilon = 1e-6;
    static constexpr f64 SmallEps  = 1e-15;

    static constexpr const char* Name = "double";
};

// Default engine precision — single for performance
using DefaultPrecision = SinglePrecision;

// ── Precision Traits ──────────────────────────────────────────────────────────

template<typename T>
struct PrecisionTraits {
    static constexpr T Epsilon    = std::numeric_limits<T>::epsilon() * 100;
    static constexpr T SmallEps   = std::numeric_limits<T>::epsilon();
};

template<>
struct PrecisionTraits<f32> {
    static constexpr f32 Epsilon  = 1e-6f;
    static constexpr f32 SmallEps = 1e-10f;
};

template<>
struct PrecisionTraits<f64> {
    static constexpr f64 Epsilon  = 1e-12;
    static constexpr f64 SmallEps = 1e-15;
};

// ── Tolerant Comparison ───────────────────────────────────────────────────────

template<typename T>
constexpr bool NearEqual(T a, T b, T eps = PrecisionTraits<T>::Epsilon) noexcept {
    T diff = a - b;
    return diff >= -eps && diff <= eps;
}

template<typename T>
constexpr bool IsFiniteValue(T v) noexcept {
    return v == v && v != std::numeric_limits<T>::infinity() && v != -std::numeric_limits<T>::infinity();
}

template<typename T>
constexpr bool IsValidScalar(T v) noexcept {
    return IsFiniteValue(v);
}

} // namespace Shape::Math::Geometry
