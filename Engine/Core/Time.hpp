#pragma once

/// @file Time.hpp
/// @brief High-resolution timing utilities.
///
/// All types use std::chrono::steady_clock for monotonic timing.
/// NEVER use wall-clock time in simulation logic — use the deterministic
/// SimulationClock from Simulation/SimulationClock.hpp instead.

#include "Core/Platform.hpp"
#include <chrono>

namespace Shape::Time {

using Clock      = std::chrono::steady_clock;
using TimePoint  = Clock::time_point;
using Duration   = Clock::duration;
using Microseconds = std::chrono::microseconds;
using Milliseconds = std::chrono::milliseconds;
using Seconds      = std::chrono::seconds;

/// Current monotonic timestamp.
SHAPE_INLINE TimePoint Now() noexcept { return Clock::now(); }

/// Microseconds elapsed since a start point.
SHAPE_INLINE i64 MicrosecondsSince(TimePoint start) noexcept {
    return std::chrono::duration_cast<Microseconds>(Now() - start).count();
}

/// Milliseconds elapsed since a start point.
SHAPE_INLINE i64 MillisecondsSince(TimePoint start) noexcept {
    return std::chrono::duration_cast<Milliseconds>(Now() - start).count();
}

/// Fractional seconds elapsed since a start point.
SHAPE_INLINE f64 SecondsSince(TimePoint start) noexcept {
    return std::chrono::duration<f64>(Now() - start).count();
}

/// Fractional seconds between two points.
SHAPE_INLINE f64 SecondsBetween(TimePoint a, TimePoint b) noexcept {
    return std::chrono::duration<f64>(b - a).count();
}

/// Microseconds between two points.
SHAPE_INLINE i64 MicrosecondsBetween(TimePoint a, TimePoint b) noexcept {
    return std::chrono::duration_cast<Microseconds>(b - a).count();
}

// ─── ScopeTimer ───────────────────────────────────────────────────────────────
/// RAII scope timer. Prints elapsed time via Logger on destruction.
/// Only active in Debug builds if SHAPE_LOGGING_ENABLED.
class ScopeTimer {
public:
    explicit ScopeTimer(const char* name) noexcept
        : m_name(name), m_start(Clock::now()) {}

    ~ScopeTimer() noexcept;

    /// Elapsed microseconds so far (non-destructive read).
    SHAPE_NODISCARD i64 ElapsedUs() const noexcept {
        return std::chrono::duration_cast<Microseconds>(Clock::now() - m_start).count();
    }

private:
    const char* m_name;
    TimePoint   m_start;
};

// ─── FrameTimer ───────────────────────────────────────────────────────────────
/// Tracks frame delta time for real-time loops (rendering, input).
/// NOT for simulation — use SimulationClock for fixed-step determinism.
class FrameTimer {
public:
    FrameTimer() noexcept : m_last(Clock::now()) {}

    /// Call once per frame. Returns elapsed seconds since last call.
    f64 Tick() noexcept {
        const TimePoint now = Clock::now();
        const f64 dt = std::chrono::duration<f64>(now - m_last).count();
        m_last = now;
        return dt;
    }

    void Reset() noexcept { m_last = Clock::now(); }

private:
    TimePoint m_last;
};

} // namespace Shape::Time

/// Profile a scope by name. Logs elapsed time on scope exit (Debug only).
#define SHAPE_PROFILE_SCOPE(name) \
    [[maybe_unused]] ::Shape::Time::ScopeTimer _shape_timer_##__COUNTER__(name)
