#pragma once

/// @file CombatSystem.hpp
/// @brief Predator-prey combat: size-scaled attacks, damage, death, energy gain.
///
/// Execution order: SystemPriority::Physics (30)
/// Frequency: Every tick.
///
/// Aggression-based targeting: creatures with high aggression and hunger
/// attack nearby smaller creatures. Damage formula:
///   damage = max(1, attack_power * 1.5 - (defense * 0.5 + size * 0.3))
///
/// DETERMINISM: spatial queries sorted by index before use.

#include "Core/Platform.hpp"
#include "Simulation/ISimulationSystem.hpp"
#include "Simulation/Spatial/SpatialGrid.hpp"
#include "ECS2/World2.hpp"

namespace Shape {

class CombatSystem final : public ISimulationSystem {
public:
    CombatSystem() = default;

    void on_register()   override;
    void on_unregister() override {}
    void update(const TickContext& ctx) override;
    int  priority() const override { return 30; }
    std::string_view name() const override { return "CombatSystem"; }

    void set_world(ECS2::World2* w) noexcept { m_world = w; }
    void set_grid(SpatialGrid* g)   noexcept { m_grid = g; }

    // ── Tuneable parameters ──────────────────────────────────────────────
    float attack_power_scale = 1.5f;   ///< Multiplier on attack_power
    float defense_scale      = 0.5f;   ///< Multiplier on defense
    float size_defense_scale = 0.3f;   ///< Size contributes to defense
    float kill_energy_gain   = 25.0f;  ///< Energy gained per kill (× prey mass)
    float color_decay_rate   = 2.0f;   ///< ColorOverride strength decay per second

private:
    ECS2::World2*  m_world = nullptr;
    SpatialGrid*   m_grid  = nullptr;

    // Find the best target for `self` near `pos`.
    // Returns EntityId::invalid() if none found.
    ECS2::EntityId find_target(ECS2::EntityId self,
                                Math::Vector2f pos,
                                float sight,
                                Simulation::DeterministicRng& rng) const;

    // Apply one attack; returns true if target was killed.
    bool perform_attack(ECS2::EntityId attacker,
                        ECS2::EntityId target,
                        const DerivedAttributes& att,
                        const DerivedAttributes& def);
};

} // namespace Shape
