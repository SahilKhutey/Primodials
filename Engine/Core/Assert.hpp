#pragma once

#include "Core/Platform.hpp"
#include "Core/Logger.hpp"

#if defined(SHAPE_PLATFORM_WINDOWS)
    #define SHAPE_DEBUGBREAK() __debugbreak()
#elif defined(SHAPE_COMPILER_GCC) || defined(SHAPE_COMPILER_CLANG)
    #define SHAPE_DEBUGBREAK() __builtin_trap()
#else
    #include <cstdlib>
    #define SHAPE_DEBUGBREAK() std::abort()
#endif

// Base panic macro
#define SHAPE_PANIC(fmt, ...) \
    do { \
        ::Shape::Logger::LogFormat(::Shape::LogLevel::Critical, __FILE__, __LINE__, "PANIC: " fmt __VA_OPT__(,) __VA_ARGS__); \
        SHAPE_DEBUGBREAK(); \
    } while (false)

#if defined(SHAPE_DEBUG)
    #define SHAPE_ASSERT(condition) \
        do { \
            if (!(condition)) { \
                ::Shape::Logger::LogFormat(::Shape::LogLevel::Critical, __FILE__, __LINE__, "Assertion Failed: ({})", #condition); \
                SHAPE_DEBUGBREAK(); \
            } \
        } while (false)

    #define SHAPE_ASSERT_MSG(condition, fmt, ...) \
        do { \
            if (!(condition)) { \
                ::Shape::Logger::LogFormat(::Shape::LogLevel::Critical, __FILE__, __LINE__, "Assertion Failed: ({}) - " fmt, #condition __VA_OPT__(,) __VA_ARGS__); \
                SHAPE_DEBUGBREAK(); \
            } \
        } while (false)
#else
    #define SHAPE_ASSERT(condition)            do { (void)(condition); } while (false)
    #define SHAPE_ASSERT_MSG(condition, ...)   do { (void)(condition); } while (false)
#endif

// ---------------------------------------------------------------------------
// SHAPE_VERIFY — always evaluates expression, asserts in Debug only
// ---------------------------------------------------------------------------
#if defined(SHAPE_DEBUG)
    #define SHAPE_VERIFY(condition) SHAPE_ASSERT(condition)
#else
    #define SHAPE_VERIFY(condition) ((void)(condition))
#endif

// ---------------------------------------------------------------------------
// SHAPE_UNREACHABLE — marks code paths that must never execute
// ---------------------------------------------------------------------------
#if defined(SHAPE_DEBUG)
    #define SHAPE_UNREACHABLE() \
        do { \
            ::Shape::Logger::LogFormat(::Shape::LogLevel::Critical, __FILE__, __LINE__, "Unreachable code reached"); \
            SHAPE_DEBUGBREAK(); \
        } while (false)
#elif defined(SHAPE_COMPILER_MSVC)
    #define SHAPE_UNREACHABLE() __assume(0)
#elif defined(SHAPE_COMPILER_GCC) || defined(SHAPE_COMPILER_CLANG)
    #define SHAPE_UNREACHABLE() __builtin_unreachable()
#else
    #define SHAPE_UNREACHABLE() ((void)0)
#endif

// ---------------------------------------------------------------------------
// SHAPE_NOT_IMPLEMENTED / SHAPE_TODO — stub placeholders
// ---------------------------------------------------------------------------
#define SHAPE_NOT_IMPLEMENTED() \
    do { \
        ::Shape::Logger::LogFormat(::Shape::LogLevel::Critical, __FILE__, __LINE__, \
            "NOT IMPLEMENTED: {}", __func__); \
        SHAPE_DEBUGBREAK(); \
    } while (false)

#define SHAPE_TODO(msg) \
    do { \
        ::Shape::Logger::LogFormat(::Shape::LogLevel::Critical, __FILE__, __LINE__, \
            "TODO: {}", msg); \
        SHAPE_DEBUGBREAK(); \
    } while (false)
