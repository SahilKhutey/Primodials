#pragma once

/// @file SimulationState.hpp
/// @brief World-level simulation state resources: bounds, environment, statistics.
/// These are stored as World2 resources (not entities).

#include "Core/Platform.hpp"
#include "Math/Include/ShapeEngine/Math/Vector2.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace Shape {

// ─── World Bounds ─────────────────────────────────────────────────────────────

/// Axis-aligned rectangular world boundary with optional toroidal wrap.
struct WorldBounds {
    Math::Vector2f min{-500.0f, -500.0f};
    Math::Vector2f max{ 500.0f,  500.0f};

    SHAPE_NODISCARD float width()  const noexcept { return max.x - min.x; }
    SHAPE_NODISCARD float height() const noexcept { return max.y - min.y; }
    SHAPE_NODISCARD Math::Vector2f center() const noexcept {
        return {(min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f};
    }

    SHAPE_NODISCARD bool contains(Math::Vector2f p) const noexcept {
        return p.x >= min.x && p.x <= max.x && p.y >= min.y && p.y <= max.y;
    }

    /// Clamp position to bounds (no-wrap version).
    void clamp(Math::Vector2f& p) const noexcept {
        p.x = std::clamp(p.x, min.x, max.x);
        p.y = std::clamp(p.y, min.y, max.y);
    }

    /// Toroidal wrap — ensures position always stays inside the world.
    void wrap(Math::Vector2f& p) const noexcept {
        const float w = width();
        const float h = height();
        while (p.x <  min.x) p.x += w;
        while (p.x >= max.x) p.x -= w;
        while (p.y <  min.y) p.y += h;
        while (p.y >= max.y) p.y -= h;
    }

    /// Shortest-path delta (toroidal metric).
    SHAPE_NODISCARD Math::Vector2f delta_wrapped(Math::Vector2f a, Math::Vector2f b) const noexcept {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        const float w = width(), h = height();
        if (dx >  w * 0.5f) dx -= w;
        if (dx < -w * 0.5f) dx += w;
        if (dy >  h * 0.5f) dy -= h;
        if (dy < -h * 0.5f) dy += h;
        return {dx, dy};
    }
};

// ─── Environment ──────────────────────────────────────────────────────────────

/// Per-tick environmental conditions that influence creature behaviour.
struct EnvironmentState {
    float temperature    = 20.0f;  ///< Celsius
    float humidity       = 0.50f;  ///< 0–1
    float pressure       = 1.00f;  ///< Atmospheres
    float oxygen         = 0.21f;  ///< Volume fraction

    u64   current_tick   = 0;
    float day_length_ticks = 14400.0f; ///< 4-hour sim-day at 60 Hz (14400 ticks)
    float season_progress  = 0.0f;     ///< 0–1 through a full year

    /// Normalised time-of-day (0 = midnight, 0.5 = noon, 1 = next midnight).
    SHAPE_NODISCARD float time_of_day() const noexcept {
        return std::fmod(static_cast<float>(current_tick) / day_length_ticks, 1.0f);
    }

    SHAPE_NODISCARD bool is_day() const noexcept { return time_of_day() < 0.5f; }

    /// Sunlight intensity 0–1 (sinusoidal ramp, peak at noon).
    SHAPE_NODISCARD float sunlight() const noexcept {
        const float t = time_of_day(); // 0–1
        const float s = std::sin(3.14159265f * t);
        return s > 0.0f ? s : 0.0f;
    }
};

// ─── Population / Simulation Statistics ──────────────────────────────────────

/// Accumulated simulation-wide statistics. Updated by EvolutionSystem.
struct SimulationStats {
    u64    total_births       = 0;
    u64    total_deaths       = 0;
    u64    total_speciations  = 0;
    u64    total_extinctions  = 0;
    usize  current_population = 0;
    usize  peak_population    = 0;
    usize  species_count      = 0;
    double avg_fitness        = 0.0;
    double avg_generation     = 0.0;
    u64    peak_population_tick = 0;
};

// ─── World State (root resource stored in World2) ────────────────────────────

/// The single world-state resource stored in World2 via world.set_resource().
struct WorldState {
    WorldBounds      bounds;
    EnvironmentState environment;
    SimulationStats  stats;
    u32              carrying_capacity = 10000; ///< Hard population cap
    bool             wrap_world        = true;  ///< Toroidal world geometry

    void tick(u64 current_tick) noexcept {
        environment.current_tick = current_tick;
        const float year_ticks = environment.day_length_ticks * 365.0f;
        environment.season_progress =
            std::fmod(static_cast<float>(current_tick), year_ticks) / year_ticks;
    }
};

} // namespace Shape
