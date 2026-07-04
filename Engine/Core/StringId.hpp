#pragma once

#include "Core/Platform.hpp"
#include <string_view>

namespace Shape {

// ─────────────────────────────────────────────────────────────────────────────
//  StringId — 64-bit FNV-1a hash of a C-string or std::string_view.
//  Same seed = same hash on all platforms (endian-independent arithmetic).
//
//  Usage:
//    StringId id = SID("FireballSpell");
//    StringId id = "FireballSpell"_sid;
//    const char* name = StringId::Lookup(id);  // debug builds only
// ─────────────────────────────────────────────────────────────────────────────

namespace Detail {

    // FNV-1a 64-bit constants
    constexpr u64 FNV_OFFSET_BASIS = 14695981039346656037ULL;
    constexpr u64 FNV_PRIME        = 1099511628211ULL;

    // Compile-time hash
    constexpr u64 fnv1a_64(const char* str, usize len) noexcept {
        u64 hash = FNV_OFFSET_BASIS;
        for (usize i = 0; i < len; ++i) {
            hash ^= static_cast<u64>(static_cast<unsigned char>(str[i]));
            hash *= FNV_PRIME;
        }
        return hash;
    }

} // namespace Detail

/// Compute a StringId from a string_view at runtime.
SHAPE_INLINE constexpr StringId MakeStringId(std::string_view sv) noexcept {
    return Detail::fnv1a_64(sv.data(), sv.size());
}

/// Compute a StringId from a null-terminated C-string at runtime.
SHAPE_INLINE constexpr StringId MakeStringId(const char* str) noexcept {
    usize len = 0;
    while (str[len] != '\0') ++len;
    return Detail::fnv1a_64(str, len);
}

} // namespace Shape

// ─────────────────────────────────────────────────────────────────────────────
//  User-defined literal:  auto id = "CreatureSpecies"_sid;
// ─────────────────────────────────────────────────────────────────────────────
constexpr StringId operator""_sid(const char* str, usize len) noexcept {
    return Shape::Detail::fnv1a_64(str, len);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Convenience macro — preferred in most engine code.
//  SID("name") evaluates at compile time when the argument is a string literal.
// ─────────────────────────────────────────────────────────────────────────────
#define SID(str) (Shape::MakeStringId(str))
