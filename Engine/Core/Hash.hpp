#pragma once

/// @file Hash.hpp
/// @brief Fast, deterministic, non-cryptographic hash functions.
///
/// All hash functions here are DETERMINISTIC — same input always produces
/// same output, across platforms and runs. Safe to use in simulation code.
/// Do NOT use std::hash<T*> in simulation — pointer addresses are non-deterministic.

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace Shape::Hash {

// ─── FNV-1a ───────────────────────────────────────────────────────────────────
// Fast, good distribution for small inputs. Compile-time capable.

constexpr u32 Fnv1a32(const void* data, usize size) noexcept {
    constexpr u32 OFFSET = 0x811C9DC5u;
    constexpr u32 PRIME  = 0x01000193u;
    u32 hash = OFFSET;
    const auto* bytes = static_cast<const u8*>(data);
    for (usize i = 0; i < size; ++i) {
        hash ^= bytes[i];
        hash *= PRIME;
    }
    return hash;
}

constexpr u32 Fnv1a32(std::string_view s) noexcept {
    return Fnv1a32(s.data(), s.size());
}

constexpr u64 Fnv1a64(const void* data, usize size) noexcept {
    constexpr u64 OFFSET = 0xCBF29CE484222325ull;
    constexpr u64 PRIME  = 0x100000001B3ull;
    u64 hash = OFFSET;
    const auto* bytes = static_cast<const u8*>(data);
    for (usize i = 0; i < size; ++i) {
        hash ^= bytes[i];
        hash *= PRIME;
    }
    return hash;
}

constexpr u64 Fnv1a64(std::string_view s) noexcept {
    return Fnv1a64(s.data(), s.size());
}

// ─── SplitMix64 mixing ────────────────────────────────────────────────────────
// Used as the seeding step for xoshiro256**. Deterministic, bijective.

constexpr u64 SplitMix64Mix(u64 x) noexcept {
    x += 0x9E3779B97F4A7C15ull;
    x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ull;
    x = (x ^ (x >> 27)) * 0x94D049BB133111EBull;
    return x ^ (x >> 31);
}

constexpr u32 SplitMix32Mix(u32 x) noexcept {
    x += 0x9E3779B9u;
    x = (x ^ (x >> 16)) * 0x85EBCA6Bu;
    x = (x ^ (x >> 13)) * 0xC2B2AE35u;
    return x ^ (x >> 16);
}

// ─── Combine ─────────────────────────────────────────────────────────────────
// Combines two hash values. Use to hash structs field-by-field.

constexpr void Combine(u64& seed, u64 value) noexcept {
    seed ^= value + 0x9E3779B97F4A7C15ull + (seed << 6) + (seed >> 2);
}

constexpr void Combine(u32& seed, u32 value) noexcept {
    seed ^= value + 0x9E3779B9u + (seed << 6) + (seed >> 2);
}

// ─── Generic hash for arithmetic types ───────────────────────────────────────

template<typename T>
constexpr u64 HashValue(T value) noexcept {
    if constexpr (sizeof(T) <= 4) {
        return SplitMix32Mix(static_cast<u32>(value));
    } else {
        return SplitMix64Mix(static_cast<u64>(value));
    }
}

/// Hash a raw byte range — deterministic string hash.
constexpr u64 HashBytes(const void* data, usize size) noexcept {
    return Fnv1a64(data, size);
}

constexpr u64 HashString(std::string_view s) noexcept {
    return Fnv1a64(s);
}

// ─── CRC32 (IEEE 802.3) ───────────────────────────────────────────────────────
// Runtime-only (table is generated on first use). Suitable for checksums.
u32 Crc32(const void* data, usize size) noexcept;
u32 Crc32(std::string_view s) noexcept;

} // namespace Shape::Hash
