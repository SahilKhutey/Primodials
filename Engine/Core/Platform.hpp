#pragma once

#include <cstdint>
#include <cstddef>

// Platform Detection
#if defined(_WIN32) || defined(_WIN64)
    #define SHAPE_PLATFORM_WINDOWS 1
#elif defined(__linux__)
    #define SHAPE_PLATFORM_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
    #define SHAPE_PLATFORM_MACOS 1
#else
    #error "Unsupported Platform!"
#endif

// Compiler Detection
#if defined(_MSC_VER)
    #define SHAPE_COMPILER_MSVC 1
#elif defined(__clang__)
    #define SHAPE_COMPILER_CLANG 1
#elif defined(__GNUC__)
    #define SHAPE_COMPILER_GCC 1
#else
    #error "Unsupported Compiler!"
#endif

// Inline keywords
#if defined(SHAPE_COMPILER_MSVC)
    #define SHAPE_INLINE __forceinline
    #define SHAPE_NOINLINE __declspec(noinline)
#elif defined(SHAPE_COMPILER_GCC) || defined(SHAPE_COMPILER_CLANG)
    #define SHAPE_INLINE inline __attribute__((always_inline))
    #define SHAPE_NOINLINE __attribute__((noinline))
#else
    #define SHAPE_INLINE inline
    #define SHAPE_NOINLINE
#endif

// Static Assert wrapper
#define SHAPE_STATIC_ASSERT static_assert

// Utility macros
#if defined(SHAPE_COMPILER_MSVC)
    #define SHAPE_NODISCARD   [[nodiscard]]
    #define SHAPE_LIKELY(x)   (x)
    #define SHAPE_UNLIKELY(x) (x)
    #define SHAPE_UNUSED(x)   (void)(x)
#elif defined(SHAPE_COMPILER_GCC) || defined(SHAPE_COMPILER_CLANG)
    #define SHAPE_NODISCARD   [[nodiscard]]
    #define SHAPE_LIKELY(x)   __builtin_expect(!!(x), 1)
    #define SHAPE_UNLIKELY(x) __builtin_expect(!!(x), 0)
    #define SHAPE_UNUSED(x)   (void)(x)
#else
    #define SHAPE_NODISCARD
    #define SHAPE_LIKELY(x)   (x)
    #define SHAPE_UNLIKELY(x) (x)
    #define SHAPE_UNUSED(x)   (void)(x)
#endif

// Debug build detection
#if !defined(NDEBUG) || defined(_DEBUG)
    #define SHAPE_DEBUG 1
#else
    #define SHAPE_RELEASE 1
#endif

// Global Types
using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;
using f64 = double;

using usize = std::size_t;

// Ensure sizes of standard types are what we expect
SHAPE_STATIC_ASSERT(sizeof(u8)  == 1, "u8 size mismatch");
SHAPE_STATIC_ASSERT(sizeof(u16) == 2, "u16 size mismatch");
SHAPE_STATIC_ASSERT(sizeof(u32) == 4, "u32 size mismatch");
SHAPE_STATIC_ASSERT(sizeof(u64) == 8, "u64 size mismatch");

SHAPE_STATIC_ASSERT(sizeof(i8)  == 1, "i8 size mismatch");
SHAPE_STATIC_ASSERT(sizeof(i16) == 2, "i16 size mismatch");
SHAPE_STATIC_ASSERT(sizeof(i32) == 4, "i32 size mismatch");
SHAPE_STATIC_ASSERT(sizeof(i64) == 8, "i64 size mismatch");

SHAPE_STATIC_ASSERT(sizeof(f32) == 4, "f32 size mismatch");
SHAPE_STATIC_ASSERT(sizeof(f64) == 8, "f64 size mismatch");

// ── StringId ─────────────────────────────────────────────────────────────────
// A 64-bit compile-time/runtime hash of a string, used everywhere names are
// compared (species names, tech IDs, sound cues, etc.).  Full type is in
// Core/StringId.hpp; the alias is declared here so other headers can use it
// without including the full implementation.
using StringId = u64;
constexpr StringId INVALID_STRING_ID = 0;

// ── Color ─────────────────────────────────────────────────────────────────────
// Lightweight RGBA color type.  Used by rendering, biology (species colours),
// and the editor palette.
struct Color {
    u8 r = 255, g = 255, b = 255, a = 255;

    constexpr Color() = default;
    constexpr Color(u8 r_, u8 g_, u8 b_, u8 a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}

    // Common presets
    static constexpr Color White()       { return {255, 255, 255, 255}; }
    static constexpr Color Black()       { return {  0,   0,   0, 255}; }
    static constexpr Color Red()         { return {255,   0,   0, 255}; }
    static constexpr Color Green()       { return {  0, 255,   0, 255}; }
    static constexpr Color Blue()        { return {  0,   0, 255, 255}; }
    static constexpr Color Transparent() { return {  0,   0,   0,   0}; }

    bool operator==(const Color& o) const { return r==o.r && g==o.g && b==o.b && a==o.a; }
    bool operator!=(const Color& o) const { return !(*this == o); }
};
