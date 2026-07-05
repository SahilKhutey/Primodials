#include "Simulation/Combat/CombatSystem.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include "Simulation/Components/LineageComponents.hpp"
#include "Simulation/SimulationState.hpp"
#include "Core/Logger.hpp"
#include <algorithm>
#include <cmath>

namespace Shape {

// ─── Registration ─────────────────────────────────────────────────────────────

void CombatSystem::on_register() {
    SHAPE_LOG_INFO("Combat", "CombatSystem registered");
}

// ─── find_target ─────────────────────────────────────────────────────────────

ECS2::EntityId CombatSystem::find_target(ECS2::EntityId self,
                                          Math::Vector2f pos,
                                          float sight,
                                          Simulation::DeterministicRng& rng) const {
    if (!m_grid) return ECS2::EntityId::invalid();

    // Gather nearby entities
    std::vector<u32> nearby;
    nearby.reserve(32);
    m_grid->query_circle(pos, sight, nearby);

    // Sort for determinism
    std::sort(nearby.begin(), nearby.end());

    // Filter to valid living creatures that are NOT self
    std::vector<ECS2::EntityId> candidates;
    for (u32 idx : nearby) {
        const ECS2::EntityId cand{idx, 0};
        if (cand == self) continue;
        if (!m_world->valid(cand)) continue;
        if (!m_world->has<CreatureState>(cand)) continue;
        if (!m_world->get<CreatureState>(cand).is_alive) continue;
        if (!m_world->has<DerivedAttributes>(cand)) continue;

        // Only attack targets >= 30% of attacker size (no point attacking specks)
        if (!m_world->has<DerivedAttributes>(self)) continue;
        const float my_size     = m_world->get<DerivedAttributes>(self).size;
        const float target_size = m_world->get<DerivedAttributes>(cand).size;
        if (target_size < my_size * 0.3f) continue;

        candidates.push_back(cand);
    }

    if (candidates.empty()) return ECS2::EntityId::invalid();

    // Pick smallest nearby (weakest prey preferred)
    std::sort(candidates.begin(), candidates.end(),
              [&](ECS2::EntityId a, ECS2::EntityId b) {
                  const float sa = m_world->get<DerivedAttributes>(a).size;
                  const float sb = m_world->get<DerivedAttributes>(b).size;
                  return sa < sb;
              });

    // Among the smallest third, pick randomly for variety
    const usize pool = std::max(usize{1}, candidates.size() / 3 + 1);
    const u32 pick   = rng.NextU32() % static_cast<u32>(pool);
    return candidates[pick];
}

// ─── perform_attack ──────────────────────────────────────────────────────────

bool CombatSystem::perform_attack(ECS2::EntityId attacker,
                                   ECS2::EntityId target,
                                   const DerivedAttributes& att,
                                   const DerivedAttributes& def) {
    if (!m_world->valid(target) || !m_world->has<CreatureState>(target)) return false;

    const float raw     = att.attack_power * attack_power_scale;
    const float shield  = def.defense * defense_scale + def.size * size_defense_scale;
    const float damage  = std::max(1.0f, raw - shield);

    auto& cs = m_world->get<CreatureState>(target);
    cs.health -= damage;

    // Red flash on target
    if (m_world->has<ColorOverride>(target)) {
        auto& co = m_world->get<ColorOverride>(target);
        co.r = 1.0f; co.g = 0.1f; co.b = 0.1f;
        co.strength = 0.7f;
    }

    // Orange flash on attacker
    if (m_world->has<ColorOverride>(attacker)) {
        auto& co = m_world->get<ColorOverride>(attacker);
        co.r = 1.0f; co.g = 0.4f; co.b = 0.0f;
        co.strength = 0.4f;
    }

    return cs.health <= 0.0f;
}

// ─── update ───────────────────────────────────────────────────────────────────

void CombatSystem::update(const TickContext& ctx) {
    if (!m_world) return;

    auto& rng = const_cast<Simulation::DeterministicRng&>(ctx.rng);
    const float dt = static_cast<float>(ctx.fixed_delta);

    // Collect attack intents (can't destroy inside for_each)
    struct AttackIntent {
        ECS2::EntityId attacker;
        ECS2::EntityId target;
    };
    std::vector<AttackIntent> intents;

    m_world->for_each<ECS2::EntityId, CreatureState, DerivedAttributes,
                      CombatStats, Position, ColorOverride>(
        [&](ECS2::EntityId e, CreatureState& cs, DerivedAttributes& da,
            CombatStats& cb, Position& pos, ColorOverride& co) {

        // Decay color flash
        if (co.strength > 0.0f) {
            co.strength -= dt * color_decay_rate;
            if (co.strength < 0.0f) co.strength = 0.0f;
        }

        if (!cs.is_alive) return;

        // Tick cooldown
        cb.cooldown_remaining -= dt;
        if (cb.cooldown_remaining > 0.0f) return;

        // Decide to attack based on aggression + hunger
        const float energy_ratio = cs.energy / std::max(1.0f, da.max_energy);
        const float hunger       = 1.0f - energy_ratio;
        const float hp_ratio     = cs.health / std::max(1.0f, da.max_health);

        // Attack decision: aggressive + hungry + healthy enough to fight
        const float attack_will  = da.aggression * 0.6f + hunger * 0.4f;
        if (attack_will < 0.35f) return;   // Not motivated
        if (hp_ratio < 0.20f)    return;   // Too wounded to fight

        // Find target
        const ECS2::EntityId target = find_target(e, pos.value, da.sight_range, rng);
        if (!target.is_valid()) return;

        // Check we are in attack range
        if (!m_world->has<Position>(target)) return;
        const Math::Vector2f tpos = m_world->get<Position>(target).value;
        const float dist = (tpos - pos.value).Length();
        if (dist > cb.attack_range) return;  // Out of range

        intents.push_back({e, target});
    });

    // Resolve attacks
    struct KillEvent {
        ECS2::EntityId attacker;
        ECS2::EntityId target;
        float prey_mass;
    };
    std::vector<KillEvent> kills;

    for (const auto& intent : intents) {
        if (!m_world->valid(intent.attacker) || !m_world->valid(intent.target)) continue;
        if (!m_world->has<DerivedAttributes>(intent.attacker)) continue;
        if (!m_world->has<DerivedAttributes>(intent.target)) continue;

        const auto& att = m_world->get<DerivedAttributes>(intent.attacker);
        const auto& def = m_world->get<DerivedAttributes>(intent.target);

        const bool killed = perform_attack(intent.attacker, intent.target, att, def);

        // Reset attacker cooldown
        if (m_world->has<CombatStats>(intent.attacker)) {
            m_world->get<CombatStats>(intent.attacker).cooldown_remaining =
                m_world->get<CombatStats>(intent.attacker).attack_cooldown;
        }

        if (killed) {
            kills.push_back({intent.attacker, intent.target, def.mass});
        }
    }

    // Process kills
    for (const auto& kill : kills) {
        if (!m_world->valid(kill.attacker) || !m_world->valid(kill.target)) continue;

        // Attacker gains energy from kill
        if (m_world->has<CreatureState>(kill.attacker)) {
            auto& cs = m_world->get<CreatureState>(kill.attacker);
            cs.energy = std::min(
                m_world->has<DerivedAttributes>(kill.attacker)
                    ? m_world->get<DerivedAttributes>(kill.attacker).max_energy : 100.0f,
                cs.energy + kill.prey_mass * kill_energy_gain
            );
        }

        // Increment kill counter
        if (m_world->has<CombatTracker>(kill.attacker)) {
            m_world->get<CombatTracker>(kill.attacker).kills++;
        }

        // Mark target as dead (removed at end of Biology tick)
        auto& target_cs = m_world->get<CreatureState>(kill.target);
        target_cs.is_alive = false;

        // Update world death counter
        auto& ws = m_world->resource<WorldState>();
        ws.stats.total_deaths++;
        ws.stats.current_population = ws.stats.current_population > 0
            ? ws.stats.current_population - 1 : 0;

        SHAPE_LOG_DEBUG("Combat", "Kill: attacker={} kills prey={} (gen={})",
                        kill.attacker.index, kill.target.index,
                        m_world->has<Ancestors>(kill.target)
                            ? m_world->get<Ancestors>(kill.target).generation : 0u);
    }
}

} // namespace Shape
