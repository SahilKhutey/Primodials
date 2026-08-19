/**
 * Primordials — Explicit SIMD Vectorization Utilities (C++23)
 *
 * AVX2 & NEON accelerated batch spatial operations.
 *
 * License: MIT
 */

#pragma once

#include <cstddef>

#if defined(__AVX2__)
#include <immintrin.h>
#endif

namespace primordials::simd {

#if defined(__AVX2__)
[[nodiscard]] inline __m256 distance_sq_batch8(
    __m256 x1, __m256 y1,
    __m256 x2, __m256 y2
) noexcept {
    const __m256 dx = _mm256_sub_ps(x1, x2);
    const __m256 dy = _mm256_sub_ps(y1, y2);
    return _mm256_add_ps(_mm256_mul_ps(dx, dx), _mm256_mul_ps(dy, dy));
}
#endif

} // namespace primordials::simd
