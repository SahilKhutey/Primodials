#pragma once

/// @file CreatureComponents.hpp
/// @brief POD ECS2 components for all simulation creatures.

#include "Core/Platform.hpp"
#include "Math/Include/ShapeEngine/Math/Vector2.hpp"
#include "ECS/SpeciesSchema.hpp"  // for SpeciesId
#include <array>

namespace Shape {

// ─── Spatial ──────────────────────────────────────────────────────────────────

/// World-space position of a creature or resource.
struct Position {
    Math::Vector2f value{0.0f, 0.0f};
};

/// World-space velocity (units per second).
struct Velocity {
    Math::Vector2f value{0.0f, 0.0f};
};

/// Orientation in radians (0 = right, CCW positive).
struct Rotation {
    float radians = 0.0f;
};

/// Internal spatial-grid registration state.
struct SpatialTag {
    bool           registered = false;
    Math::Vector2f last_pos{0.0f, 0.0f};
    u32            grid_key  = 0;
};

// ─── Creature lifecycle ───────────────────────────────────────────────────────

struct CreatureState {
    SpeciesId species_id   = INVALID_SPECIES_ID;
    float     health       = 100.0f;
    float     energy       = 100.0f;
    u32       age_ticks    = 0;
    u32       generation   = 0;
    bool      is_alive     = true;
    bool      is_mature    = false;
    bool      wants_food   = false;
    u8        _pad[1]{};
};

struct ReproductionState {
    u32   cooldown_ticks = 0;
    u32   offspring_count= 0;
    bool  ready          = false;
    u8    _pad[3]{};
};

// ─── Genetics ─────────────────────────────────────────────────────────────────

struct CreatureGenome {
    static constexpr u8 GENE_COUNT = 16;
    std::array<float, GENE_COUNT> genes{};
    u64  lineage_hash  = 0;
    u32  parent_a_hash = 0;
    u32  parent_b_hash = 0;
};

struct FitnessScore {
    float value            = 0.0f;
    float age_factor       = 0.0f;
    float energy_factor    = 0.0f;
    float offspring_factor = 0.0f;
    u32   last_update_tick = 0;
};

// ─── AI / Movement ────────────────────────────────────────────────────────────

enum class CreatureAction : u8 {
    Idle       = 0,
    SeekFood   = 1,
    Eat        = 2,
    Flee       = 3,
    Reproduce  = 4,
    Wander     = 5,
};

struct AIState {
    CreatureAction action        = CreatureAction::Wander;
    u32            target_idx    = ~u32{0};
    float          decision_timer= 0.0f;
    float          wander_angle  = 0.0f;
};

} // namespace Shape
