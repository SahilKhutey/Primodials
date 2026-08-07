#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  Constants.hpp
//  Geometry-specific mathematical constants.
//
//  Separate from the engine's global Math constants to allow
//  precision-specific specialization per primitive.
// ─────────────────────────────────────────────────────────────────────────────

#include "Core/Platform.hpp"
#include "Math/MathUtils.hpp"

namespace Shape::Math::Geometry {

// ── Tolerance Constants ───────────────────────────────────────────────────────
inline constexpr f32 GEO_EPSILON_F  = 1e-6f;
inline constexpr f64 GEO_EPSILON_D  = 1e-12;

// ── Geometric Constants ───────────────────────────────────────────────────────
inline constexpr f64 GEO_PI         = 3.14159265358979323846;
inline constexpr f64 GEO_TWO_PI     = 6.28318530717958647692;
inline constexpr f64 GEO_HALF_PI    = 1.57079632679489661923;
inline constexpr f64 GEO_INV_PI     = 0.31830988618379067154;
inline constexpr f64 GEO_SQRT2      = 1.41421356237309504880;
inline constexpr f64 GEO_SQRT3      = 1.73205080756887729353;
inline constexpr f64 GEO_GOLDEN     = 1.61803398874989484820; // φ (phi)

// ── Area / Volume Factors ─────────────────────────────────────────────────────
inline constexpr f64 GEO_SPHERE_AREA_FACTOR   = 4.0 * GEO_PI;       // 4π
inline constexpr f64 GEO_SPHERE_VOLUME_FACTOR = (4.0 / 3.0) * GEO_PI; // (4/3)π
inline constexpr f64 GEO_CIRCLE_AREA_FACTOR   = GEO_PI;              // π

// ── Precision-Dependent Epsilon Selector ──────────────────────────────────────
template<typename T>
struct GeoEpsilon {
    static constexpr T value = static_cast<T>(1e-6);
};

template<>
struct GeoEpsilon<f32> {
    static constexpr f32 value = 1e-6f;
};

template<>
struct GeoEpsilon<f64> {
    static constexpr f64 value = 1e-12;
};

template<typename T>
inline constexpr T GeoEpsilon_v = GeoEpsilon<T>::value;

// ── Maximum Iteration Guard (for iterative algorithms) ────────────────────────
inline constexpr u32 GEO_MAX_ITERATIONS = 64;

// ── Winding Order ─────────────────────────────────────────────────────────────
enum class WindingOrder : u8 {
    CounterClockwise = 0, // Right-hand rule (default)
    Clockwise        = 1,
};

// ── Coordinate Space Tags ─────────────────────────────────────────────────────
// Used by geometry algorithms to document and enforce frame assumptions.
struct LocalSpace  {};   // Object-local frame
struct ParentSpace {};   // Parent object frame (scene graph)
struct WorldSpace  {};   // World origin frame
struct ECEFSpace   {};   // Earth-Centered Earth-Fixed
struct ECISpace    {};   // Earth-Centered Inertial
struct GeodeticSpace {}; // Lat/Lon/Alt (WGS-84)

} // namespace Shape::Math::Geometry
