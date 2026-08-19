/**
 * Primordials — PCG-XSH-RR Random Number Generator (C++23)
 *
 * Fast, statistically robust, seedable PRNG.
 *
 * License: MIT
 */

#pragma once

#include <cstdint>
#include <limits>
#include <cmath>

namespace primordials {

class Random {
public:
    using result_type = std::uint32_t;

    constexpr Random() noexcept : state_(0x853c49e6748fea9bULL), inc_(0xda3e39cb94b95bdbULL) {}
    explicit Random(std::uint64_t seed) noexcept { seed_state(seed); }

    void seed_state(std::uint64_t seed) noexcept {
        state_ = 0u;
        inc_ = (seed << 1u) | 1u;
        next();
        state_ += seed;
        next();
    }

    [[nodiscard]] result_type next() noexcept {
        const std::uint64_t oldstate = state_;
        state_ = oldstate * 6364136223846793005ULL + inc_;
        const std::uint32_t xorshifted = static_cast<std::uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
        const std::uint32_t rot = static_cast<std::uint32_t>(oldstate >> 59u);
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    }

    [[nodiscard]] result_type operator()() noexcept { return next(); }

    [[nodiscard]] float unit_float() noexcept {
        return next() * (1.0f / 4294967296.0f);
    }

    [[nodiscard]] float range_float(float min, float max) noexcept {
        return min + (max - min) * unit_float();
    }

    [[nodiscard]] int range_int(int min, int max) noexcept {
        return min + static_cast<int>(next() % static_cast<unsigned>(max - min + 1));
    }

    [[nodiscard]] float range_angle() noexcept {
        return unit_float() * 6.283185307179586f;
    }

    [[nodiscard]] bool chance(float probability) noexcept {
        return unit_float() < probability;
    }

private:
    std::uint64_t state_;
    std::uint64_t inc_;
};

} // namespace primordials
