#pragma once

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"
#include "Math/Vec2.hpp"
#include "Math/Vec3.hpp"
#include <span>
#include <cmath>

namespace Shape::Simulation {

// ─────────────────────────────────────────────────────────────────────────────
//  DeterministicRng — xoshiro256** PRNG
//
//  DETERMINISM CONTRACT:
//    - Same seed → identical sequence on all platforms / compilers / OSes.
//    - No global state. Every consumer receives it as an explicit parameter.
//    - Use fork(stream_id) to derive independent child streams without
//      advancing the parent's state (via SplitMix64 seeded from parent state).
//    - Save/restore state() for snapshot / replay.
//
//  References:
//    Blackman & Vigna, "Scrambled Linear Pseudorandom Number Generators"
//    https://prng.di.unimi.it/xoshiro256starstar.c
// ─────────────────────────────────────────────────────────────────────────────
class DeterministicRng {
public:
    using ResultType = u64;

    // ── Construction ─────────────────────────────────────────────────────────

    /// Construct from a u64 seed (SplitMix64 used to expand it into 256 bits).
    explicit DeterministicRng(u64 seed) { SeedFrom(seed); }

    DeterministicRng() : DeterministicRng(12345678901234567ULL) {}

    // ── Raw generation ───────────────────────────────────────────────────────

    u64 NextU64() {
        const u64 result = Rotl(m_S[1] * 5, 7) * 9;
        const u64 t      = m_S[1] << 17;
        m_S[2] ^= m_S[0];
        m_S[3] ^= m_S[1];
        m_S[1] ^= m_S[2];
        m_S[0] ^= m_S[3];
        m_S[2] ^= t;
        m_S[3]  = Rotl(m_S[3], 45);
        return result;
    }

    u32 NextU32() { return static_cast<u32>(NextU64() >> 32); }

    // ── Floating-point helpers ────────────────────────────────────────────────

    /// [0, 1)  — uniform float
    f32 NextF32() {
        // Use top 24 bits for mantissa precision
        return static_cast<f32>(NextU64() >> 40) * (1.0f / 16777216.0f);
    }

    /// [0, 1)  — uniform double
    f64 NextF64() {
        return static_cast<f64>(NextU64() >> 11) * (1.0 / 9007199254740992.0);
    }

    // ── Range helpers ─────────────────────────────────────────────────────────

    /// [min, max] inclusive integer range
    i32 NextRange(i32 min_, i32 max_) {
        SHAPE_ASSERT_MSG(max_ >= min_, "DeterministicRng::NextRange: max < min");
        u64 range = static_cast<u64>(max_ - min_) + 1ULL;
        return min_ + static_cast<i32>(NextU64() % range);
    }

    /// [min, max) float range
    f32 NextRangeF32(f32 min_, f32 max_) {
        return min_ + NextF32() * (max_ - min_);
    }

    f64 NextRangeF64(f64 min_, f64 max_) {
        return min_ + NextF64() * (max_ - min_);
    }

    // ── Boolean ──────────────────────────────────────────────────────────────

    /// Returns true with probability p (clamped to [0,1]).
    bool NextBool(f32 probability = 0.5f) {
        return NextF32() < probability;
    }

    // ── Geometric helpers ─────────────────────────────────────────────────────

    /// Uniform point inside the unit disk (radius ≤ 1).
    Math::Vec2f NextVec2InUnitDisk() {
        // Rejection sampling — average 1.27 iterations
        while (true) {
            f32 x = NextRangeF32(-1.0f, 1.0f);
            f32 y = NextRangeF32(-1.0f, 1.0f);
            if (x*x + y*y <= 1.0f) return { x, y };
        }
    }

    /// Uniform point inside the unit square [0,1)²
    Math::Vec2f NextVec2InUnitSquare() {
        return { NextF32(), NextF32() };
    }

    /// Uniform direction on the unit circle
    Math::Vec2f NextVec2Direction() {
        f32 angle = NextF32() * 6.28318530717958647692f;
        return { std::cos(angle), std::sin(angle) };
    }

    /// Uniform point on the unit sphere surface
    Math::Vec3f NextVec3InUnitSphere() {
        while (true) {
            f32 x = NextRangeF32(-1.0f, 1.0f);
            f32 y = NextRangeF32(-1.0f, 1.0f);
            f32 z = NextRangeF32(-1.0f, 1.0f);
            if (x*x + y*y + z*z <= 1.0f) return { x, y, z };
        }
    }

    Math::Vec3f NextVec3InUnitCube() {
        return { NextF32(), NextF32(), NextF32() };
    }

    // ── State management ──────────────────────────────────────────────────────

    /// Full 256-bit state — save for snapshots / replay.
    struct State { u64 s[4]; };

    State GetState() const { return { { m_S[0], m_S[1], m_S[2], m_S[3] } }; }
    void  SetState(const State& st) {
        m_S[0]=st.s[0]; m_S[1]=st.s[1]; m_S[2]=st.s[2]; m_S[3]=st.s[3];
    }

    /// Discard n values from the stream (advances state without storing results).
    void Discard(u64 n) { while (n--) NextU64(); }

    // ── Shuffle ───────────────────────────────────────────────────────────────

    template<typename It>
    void Shuffle(It begin, It end) {
        auto n = static_cast<i64>(end - begin);
        for (i64 i = n - 1; i > 0; --i) {
            i64 j = static_cast<i64>(NextU64() % static_cast<u64>(i + 1));
            using std::swap;
            swap(*(begin + i), *(begin + j));
        }
    }

    // ── Forking ───────────────────────────────────────────────────────────────
    // Derive an independent child RNG without advancing the parent stream.
    // The child seed is uniquely determined by the parent's current state and
    // the stream_id — so two forks with different IDs will diverge.

    DeterministicRng Fork(u32 stream_id) const {
        // Combine current state with stream_id via SplitMix64
        u64 seed = m_S[0] ^ m_S[1] ^ m_S[2] ^ m_S[3];
        seed ^= static_cast<u64>(stream_id) * 6364136223846793005ULL;
        return DeterministicRng(SplitMix64(seed));
    }

private:
    u64 m_S[4]{};

    static u64 Rotl(u64 x, int k) {
        return (x << k) | (x >> (64 - k));
    }

    /// SplitMix64 — used to expand a single seed into the 256-bit state.
    static u64 SplitMix64(u64 x) {
        x += 0x9e3779b97f4a7c15ULL;
        x  = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x  = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    void SeedFrom(u64 seed) {
        // Run SplitMix64 four times to get independent 64-bit words
        m_S[0] = SplitMix64(seed);
        m_S[1] = SplitMix64(m_S[0]);
        m_S[2] = SplitMix64(m_S[1]);
        m_S[3] = SplitMix64(m_S[2]);
    }
};

} // namespace Shape::Simulation

// Pull into Shape:: for convenience
namespace Shape {
    using Simulation::DeterministicRng;
}
