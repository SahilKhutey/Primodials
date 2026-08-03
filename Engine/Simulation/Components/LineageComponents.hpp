#pragma once

/// @file LineageComponents.hpp
/// @brief Phase D ECS2 components for lineage, combat, reproduction, and
///        genome-driven derived attributes.

#include "Core/Platform.hpp"
#include "ECS2/EntityId.hpp"
#include <array>

namespace Shape {

// ─── Ancestor record ─────────────────────────────────────────────────────────

/// Parent and grandparent entity handles — for lineage visualisation.
/// Handles can become invalid (creature dies) — always guard with world.valid().
struct Ancestors {
    ECS2::EntityId mother{};
    ECS2::EntityId father{};
    ECS2::EntityId grandmother_m{};
    ECS2::EntityId grandfather_m{};
    ECS2::EntityId grandmother_p{};
    ECS2::EntityId grandfather_p{};
    u32  generation = 0;
    u8   _pad[4]{};
};

// ─── Derived attributes cache ─────────────────────────────────────────────────

/// Cached per-creature attribute snapshot computed by GenomeDecoder at spawn.
/// Read by Combat, Reproduction, AI, and Rendering systems.
struct DerivedAttributes {
    float max_health        = 100.0f;
    float max_energy        = 100.0f;
    float max_speed         = 50.0f;
    float mass              = 1.0f;
    float size              = 7.0f;     ///< Circumradius used for rendering
    float aggression        = 0.30f;
    float fear              = 0.30f;
    float metabolism        = 0.50f;
    float fertility         = 0.50f;
    float iq                = 50.0f;
    float lifespan          = 4800.0f;
    float disease_resistance= 0.50f;
    float regen_rate        = 0.10f;
    float attack_power      = 10.0f;
    float defense           = 5.0f;
    float digestion_eff     = 0.70f;
    float sight_range       = 100.0f;
    float gestation_period  = 60.0f;
    // Genome-decoded colors (blended from species primary + gene deltas)
    float color_r           = 1.0f;
    float color_g           = 1.0f;
    float color_b           = 1.0f;
};

// ─── Combat components ────────────────────────────────────────────────────────

/// Per-creature combat parameters (from DerivedAttributes at spawn).
struct CombatStats {
    float attack_power      = 10.0f;
    float defense           = 5.0f;
    float attack_range      = 12.0f;  ///< World-space distance
    float attack_cooldown   = 1.5f;   ///< Seconds between attacks
    float cooldown_remaining= 0.0f;   ///< Current remaining cooldown
};

/// Lifetime combat and reproduction statistics for a creature.
struct CombatTracker {
    u32 kills               = 0;
    u32 deaths_from_combat  = 0;
    u32 offspring_born      = 0;
};

// ─── Reproduction components ──────────────────────────────────────────────────

enum class MatingState : u8 {
    None      = 0,
    Seeking   = 1,
    Gestating = 2,
};

/// Per-creature reproduction state.
struct MatingStatus {
    MatingState    state            = MatingState::None;
    u8             _pad[3]{};
    float          cooldown         = 0.0f;  ///< Seconds until can mate again
    float          gestation_timer  = 0.0f;  ///< Seconds elapsed in gestation
    float          gestation_needed = 0.0f;  ///< Total gestation duration
    ECS2::EntityId partner{};
};

// ─── Visual feedback ──────────────────────────────────────────────────────────

/// Temporary color override for combat flash / birth glow.
/// Rendered color = lerp(genome_color, override_color, strength).
struct ColorOverride {
    float r        = 1.0f;
    float g        = 1.0f;
    float b        = 1.0f;
    float strength = 0.0f;  ///< 0 = no override, 1 = full override (decays each tick)
};

} // namespace Shape
