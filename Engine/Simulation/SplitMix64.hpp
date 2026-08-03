#pragma once

/// @file SplitMix64.hpp
/// @brief SplitMix64 deterministic PRNG.
///
/// SplitMix64 is used as the SEEDING step for xoshiro256**, not as a
/// primary simulation RNG. It produces a high-quality 64-bit hash from
/// a monotonically incrementing counter, suitable for initializing state.
///
/// Properties: Period = 2^64, passes BigCrush, NOT cryptographic.
///
/// Reference: Sebastiano Vigna — https://prng.di.unimi.it/splitmix64.c

#include "Core/Platform.hpp"

namespace Shape::Random {

class SplitMix64 {
public:
    explicit SplitMix64(u64 seed) noexcept : m_state(seed) {}

    /// Generate next 64-bit value.
    SHAPE_NODISCARD u64 Next() noexcept {
        m_state += 0x9E3779B97F4A7C15ull;
        u64 z = m_state;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
        return z ^ (z >> 31);
    }

    /// Generate a u32 from the upper 32 bits.
    SHAPE_NODISCARD u32 Next32() noexcept {
        return static_cast<u32>(Next() >> 32);
    }

    /// Fill a buffer with deterministic seed values for xoshiro256 initialization.
    void FillSeed(u64* out, usize count) noexcept {
        for (usize i = 0; i < count; ++i) out[i] = Next();
    }

    SHAPE_NODISCARD u64 State() const noexcept { return m_state; }
    void SetState(u64 s) noexcept { m_state = s; }

private:
    u64 m_state;
};

} // namespace Shape::Random
