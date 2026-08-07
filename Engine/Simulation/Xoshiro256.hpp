#pragma once

/// @file Xoshiro256.hpp
/// @brief xoshiro256** deterministic PRNG.
///
/// xoshiro256** is the recommended general-purpose PRNG for simulations.
/// It is fast, has excellent statistical properties, a period of 2^256-1,
/// and supports deterministic jump-ahead (for parallel streams).
///
/// Use DeterministicRng (which wraps this) for all simulation code.
/// This header provides the raw type for systems that need direct control.
///
/// Reference: Blackman & Vigna — https://prng.di.unimi.it/xoshiro256starstar.c

#include "Core/Platform.hpp"
#include "Simulation/SplitMix64.hpp"
#include <array>

namespace Shape::Random {

class Xoshiro256 {
public:
    /// Seed from a single 64-bit value via SplitMix64 expansion.
    explicit Xoshiro256(u64 seed) noexcept {
        SplitMix64 sm(seed);
        sm.FillSeed(m_state, 4);
    }

    /// Seed from a full 256-bit state (for deserialization/replay).
    Xoshiro256(u64 s0, u64 s1, u64 s2, u64 s3) noexcept
        : m_state{s0, s1, s2, s3} {}

    // ── Generation ────────────────────────────────────────────────────────────

    /// Generate next u64. Core operation.
    SHAPE_NODISCARD u64 Next() noexcept {
        const u64 result = Rotl(m_state[1] * 5, 7) * 9;
        const u64 t = m_state[1] << 17;
        m_state[2] ^= m_state[0];
        m_state[3] ^= m_state[1];
        m_state[1] ^= m_state[2];
        m_state[0] ^= m_state[3];
        m_state[2] ^= t;
        m_state[3] = Rotl(m_state[3], 45);
        return result;
    }

    /// Generate u32 from upper bits.
    SHAPE_NODISCARD u32 Next32() noexcept {
        return static_cast<u32>(Next() >> 32);
    }

    /// Generate f32 in [0, 1).
    SHAPE_NODISCARD f32 NextF32() noexcept {
        // Use 23 bits of mantissa for [0, 1)
        const u32 v = Next32();
        return static_cast<f32>(v >> 9) * (1.0f / (1u << 23));
    }

    /// Generate f64 in [0, 1).
    SHAPE_NODISCARD f64 NextF64() noexcept {
        return static_cast<f64>(Next() >> 11) * (1.0 / (1ull << 53));
    }

    /// Generate f32 in [min, max).
    SHAPE_NODISCARD f32 RangeF32(f32 min, f32 max) noexcept {
        return min + NextF32() * (max - min);
    }

    /// Generate u64 in [0, bound).
    SHAPE_NODISCARD u64 RangeU64(u64 bound) noexcept {
        if (bound == 0) return 0;
        const u64 threshold = (~bound + 1) % bound;
        for (;;) {
            const u64 r = Next();
            if (r >= threshold) return r % bound;
        }
    }

    /// Generate bool with probability p in [0, 1].
    SHAPE_NODISCARD bool NextBool(f32 p = 0.5f) noexcept {
        return NextF32() < p;
    }

    // ── Jump (2^128 steps ahead — for parallel streams) ───────────────────────

    void Jump() noexcept {
        static constexpr u64 JUMP[] = {
            0x180EC6D33CFD0ABAll, 0xD5A61266F0C9392Cll,
            0xA9582618E03FC9AAll, 0x39ABDC4529B1661Cll
        };
        u64 s0=0, s1=0, s2=0, s3=0;
        for (usize i=0; i<4; ++i) {
            for (usize b=0; b<64; ++b) {
                if (JUMP[i] & (u64{1}<<b)) {
                    s0 ^= m_state[0]; s1 ^= m_state[1];
                    s2 ^= m_state[2]; s3 ^= m_state[3];
                }
                Next();
            }
        }
        m_state[0]=s0; m_state[1]=s1; m_state[2]=s2; m_state[3]=s3;
    }

    // ── State ─────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD u64 StateWord(usize i) const noexcept { return m_state[i]; }
    void SetState(u64 s0, u64 s1, u64 s2, u64 s3) noexcept {
        m_state[0]=s0; m_state[1]=s1; m_state[2]=s2; m_state[3]=s3;
    }

private:
    static u64 Rotl(u64 x, int k) noexcept {
        return (x << k) | (x >> (64 - k));
    }

    u64 m_state[4];
};

} // namespace Shape::Random
