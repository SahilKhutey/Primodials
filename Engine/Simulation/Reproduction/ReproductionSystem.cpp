#include "Simulation/Reproduction/ReproductionSystem.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include "Simulation/Components/LineageComponents.hpp"
#include "Simulation/Genetics/GeneticsSystem.hpp"
#include "Simulation/Genome/GenomeDecoder.hpp"
#include "Simulation/SimulationState.hpp"
#include "ECS/SpeciesSchema.hpp"
#include "Core/Logger.hpp"
#include <algorithm>
#include <cmath>

namespace Shape {

// ─── Registration ─────────────────────────────────────────────────────────────

void ReproductionSystem::on_register() {
    SHAPE_LOG_INFO("Reproduction", "ReproductionSystem registered");
}

// ─── Main update ──────────────────────────────────────────────────────────────

void ReproductionSystem::update(const TickContext& ctx) {
    if (!m_world) return;

    auto& rng = const_cast<Simulation::DeterministicRng&>(ctx.rng);
    update_gestations(ctx, rng);

    // ── Mate-seeking pass ──────────────────────────────────────────────────────
    std::vector<ECS2::EntityId> candidates;
    candidates.reserve(16);

    // Collect entities that are eligible to mate this tick.
    // We can't call world.create() inside for_each (structural change),
    // so gestation begins next frame.
    struct MatingRequest {
        ECS2::EntityId self;
        ECS2::EntityId partner;
    };
    std::vector<MatingRequest> requests;

    m_world->for_each<ECS2::EntityId, CreatureState, MatingStatus, DerivedAttributes, Position>(
        [&](ECS2::EntityId e, CreatureState& cs, MatingStatus& ms,
            DerivedAttributes& da, Position& pos) {

        if (!cs.is_alive) return;
        if (ms.state != MatingState::None) return;
        if (ms.cooldown > 0.0f) { ms.cooldown -= static_cast<float>(ctx.fixed_delta); return; }

        // Age check: must be mature (5% of lifespan)
        if (static_cast<float>(cs.age_ticks) < da.lifespan * 0.05f) return;

        // Energy check
        if (cs.energy < da.max_energy * min_energy_frac) return;

        // Find nearby mates
        candidates.clear();
        const float radius = mate_search_radius + da.iq * 0.3f;
        find_mates(e, pos.value, cs.species_id, radius, candidates);
        if (candidates.empty()) return;

        // Pick a mate using deterministic RNG
        const u32 idx = static_cast<u32>(rng.NextU32() % static_cast<u32>(candidates.size()));
        requests.push_back({e, candidates[idx]});
    });

    // Process requests (outside for_each to avoid structural hazards)
    for (const auto& req : requests) {
        if (!m_world->valid(req.self) || !m_world->valid(req.partner)) continue;
        // Re-check partner is still free
        if (!m_world->has<MatingStatus>(req.partner)) continue;
        if (m_world->get<MatingStatus>(req.partner).state != MatingState::None) continue;
        begin_gestation(req.self, req.partner, rng);
    }
}

// ─── find_mates ───────────────────────────────────────────────────────────────

void ReproductionSystem::find_mates(ECS2::EntityId self,
                                     Math::Vector2f pos,
                                     SpeciesId sid,
                                     float radius,
                                     std::vector<ECS2::EntityId>& out) const {
    if (!m_grid) return;

    // Collect raw indices from spatial grid
    std::vector<u32> nearby_idx;
    nearby_idx.reserve(32);
    m_grid->query_circle(pos, radius, nearby_idx);

    // Resolve to EntityIds via world iteration — we scan candidates for match
    // (SpatialGrid stores u32 indices; we use entity.index for matching).
    // Sort for determinism before returning.
    std::vector<u32> sorted_idx = nearby_idx;
    std::stable_sort(sorted_idx.begin(), sorted_idx.end());

    for (u32 idx : sorted_idx) {
        // Attempt to reconstruct EntityId (generation = 0 for lookup; World2
        // only uses the index for validity in current design).
        // We iterate nearby and cross-check via for_each isn't viable here;
        // instead, we use a heuristic: store entity index == entity pool index.
        const ECS2::EntityId candidate{idx, 0};
        if (!m_world->valid(candidate)) continue;
        if (candidate == self) continue;
        if (!m_world->has<CreatureState>(candidate)) continue;
        if (!m_world->has<MatingStatus>(candidate)) continue;

        const auto& cs2 = m_world->get<CreatureState>(candidate);
        if (!cs2.is_alive || cs2.species_id != sid) continue;

        const auto& ms2 = m_world->get<MatingStatus>(candidate);
        if (ms2.state != MatingState::None || ms2.cooldown > 0.0f) continue;

        if (m_world->has<DerivedAttributes>(candidate)) {
            const auto& da2 = m_world->get<DerivedAttributes>(candidate);
            if (!m_world->has<CreatureState>(candidate)) continue;
            const auto& cs3 = m_world->get<CreatureState>(candidate);
            if (cs3.energy < da2.max_energy * min_energy_frac) continue;
        }

        out.push_back(candidate);
    }
}

// ─── begin_gestation ─────────────────────────────────────────────────────────

void ReproductionSystem::begin_gestation(ECS2::EntityId mother,
                                          ECS2::EntityId father,
                                          Rng& rng) {
    if (!m_world->valid(mother) || !m_world->valid(father)) return;
    if (!m_world->has<CreatureGenome>(mother) || !m_world->has<CreatureGenome>(father)) return;

    const auto& genome_m = m_world->get<CreatureGenome>(mother);
    const auto& genome_f = m_world->get<CreatureGenome>(father);

    // Crossover + mutate using Phase C GeneticsSystem utilities
    const float mutation_rate = m_world->has<DerivedAttributes>(mother)
        ? m_world->get<DerivedAttributes>(mother).metabolism * 0.02f  // metabolic cost drives mutation
        : 0.02f;

    CreatureGenome child = GeneticsSystem::crossover(genome_m, genome_f, rng);
    GeneticsSystem::mutate(child, mutation_rate, 0.1f, rng);

    // Tag mother as gestating
    auto& ms_m = m_world->get<MatingStatus>(mother);
    const float gestation_time = m_world->has<DerivedAttributes>(mother)
        ? m_world->get<DerivedAttributes>(mother).gestation_period : 60.0f;
    ms_m.state            = MatingState::Gestating;
    ms_m.partner          = father;
    ms_m.gestation_timer  = 0.0f;
    ms_m.gestation_needed = gestation_time;

    // Store child genome temporarily in mother's genome slot (swapped back on birth)
    // We add a temporary component to mother
    m_world->add<CreatureGenome>(mother, child);  // Overwrites with child genome

    // Tag father with post-mating cooldown
    if (m_world->has<MatingStatus>(father)) {
        auto& ms_f = m_world->get<MatingStatus>(father);
        ms_f.cooldown = gestation_time * 1.5f;
    }
}

// ─── update_gestations ────────────────────────────────────────────────────────

void ReproductionSystem::update_gestations(const TickContext& ctx, Rng& rng) {
    struct BirthEvent {
        ECS2::EntityId mother;
        ECS2::EntityId father;
        CreatureGenome child_genome;
    };
    std::vector<BirthEvent> births;

    // Tick gestation timers
    m_world->for_each<ECS2::EntityId, MatingStatus, CreatureGenome>(
        [&](ECS2::EntityId e, MatingStatus& ms, CreatureGenome& genome) {
        if (ms.state != MatingState::Gestating) return;
        ms.gestation_timer += static_cast<float>(ctx.fixed_delta);
        if (ms.gestation_timer >= ms.gestation_needed) {
            births.push_back({e, ms.partner, genome});
        }
    });

    // Process births (outside for_each)
    for (auto& ev : births) {
        if (!m_world->valid(ev.mother)) continue;

        spawn_offspring(ev.mother, ev.father, ev.child_genome, rng);

        // Reset mother
        if (m_world->has<MatingStatus>(ev.mother)) {
            auto& ms = m_world->get<MatingStatus>(ev.mother);
            ms.state           = MatingState::None;
            ms.partner         = ECS2::EntityId::invalid();
            ms.gestation_timer = 0.0f;
            ms.cooldown        = 300.0f;  // 5 sim-second cooldown
        }

        // Deduct energy from mother
        if (m_world->has<CreatureState>(ev.mother)) {
            auto& cs = m_world->get<CreatureState>(ev.mother);
            cs.energy = std::max(0.0f, cs.energy - birth_energy_cost);
        }

        // Update tracker
        if (m_world->has<CombatTracker>(ev.mother)) {
            m_world->get<CombatTracker>(ev.mother).offspring_born++;
        }
        if (m_world->valid(ev.father) && m_world->has<CombatTracker>(ev.father)) {
            m_world->get<CombatTracker>(ev.father).offspring_born++;
        }

        // Update world stats
        auto& ws = m_world->resource<WorldState>();
        ws.stats.total_births++;
        ws.stats.current_population++;
    }
}

// ─── spawn_offspring ─────────────────────────────────────────────────────────

ECS2::EntityId ReproductionSystem::spawn_offspring(ECS2::EntityId mother,
                                                     ECS2::EntityId father,
                                                     const CreatureGenome& child_genome,
                                                     Rng& rng) {
    if (!m_world->valid(mother)) return ECS2::EntityId::invalid();
    if (!m_world->has<CreatureState>(mother)) return ECS2::EntityId::invalid();

    const SpeciesId sid = m_world->get<CreatureState>(mother).species_id;

    // Get spawn position: near mother with tiny jitter
    Math::Vector2f spawn_pos{0.0f, 0.0f};
    if (m_world->has<Position>(mother)) {
        spawn_pos = m_world->get<Position>(mother).value;
    }
    spawn_pos.x += (rng.NextF32() - 0.5f) * 6.0f;
    spawn_pos.y += (rng.NextF32() - 0.5f) * 6.0f;

    // Resolve species for genome → stats decoding
    // (We use a default Species if registry not available)
    Species default_sp;
    const Species* sp = &default_sp;
    // TODO: inject SpeciesRegistry in Phase D finalisation

    // Decode genome → stats
    const auto stats = Simulation::Genome::apply_genome(*sp, child_genome);

    // Create entity
    const ECS2::EntityId child = m_world->create();

    m_world->add<Position>(child, {spawn_pos});
    m_world->add<Velocity>(child, {{0.0f, 0.0f}});
    m_world->add<Rotation>(child, {0.0f});
    m_world->add<SpatialTag>(child);

    CreatureState cs;
    cs.species_id = sid;
    cs.health     = stats.max_health * 0.6f;
    cs.energy     = stats.max_energy * 0.5f;
    cs.age_ticks  = 0;
    cs.generation = m_world->has<Ancestors>(mother)
        ? m_world->get<Ancestors>(mother).generation + 1 : 1;
    cs.is_alive   = true;
    cs.is_mature  = false;
    cs.wants_food = true;
    m_world->add<CreatureState>(child, cs);

    m_world->add<ReproductionState>(child, {0, 0, false});
    m_world->add<CreatureGenome>(child, child_genome);
    m_world->add<FitnessScore>(child);
    m_world->add<AIState>(child);
    m_world->add<MatingStatus>(child);
    m_world->add<CombatTracker>(child);

    // Lineage
    Ancestors anc;
    anc.mother     = mother;
    anc.father     = father;
    anc.generation = cs.generation;
    if (m_world->has<Ancestors>(mother)) {
        const auto& pm = m_world->get<Ancestors>(mother);
        anc.grandmother_m = pm.mother;
        anc.grandfather_m = pm.father;
    }
    if (m_world->valid(father) && m_world->has<Ancestors>(father)) {
        const auto& pf = m_world->get<Ancestors>(father);
        anc.grandmother_p = pf.mother;
        anc.grandfather_p = pf.father;
    }
    m_world->add<Ancestors>(child, anc);

    // Derived attributes cache
    DerivedAttributes da;
    da.max_health   = stats.max_health;
    da.max_energy   = stats.max_energy;
    da.max_speed    = stats.max_speed;
    da.mass         = stats.mass;
    da.size         = stats.size;
    da.aggression   = stats.aggression;
    da.fear         = stats.fear;
    da.metabolism   = stats.metabolism;
    da.fertility    = stats.fertility;
    da.iq           = stats.iq;
    da.lifespan     = stats.lifespan;
    da.disease_resistance = stats.disease_resistance;
    da.regen_rate   = stats.regen_rate;
    da.attack_power = stats.attack_power;
    da.defense      = stats.defense;
    da.digestion_eff= stats.digestion_eff;
    da.sight_range  = stats.sight_range;
    da.gestation_period = stats.gestation_period;
    da.color_r      = stats.color_r;
    da.color_g      = stats.color_g;
    da.color_b      = stats.color_b;
    m_world->add<DerivedAttributes>(child, da);

    CombatStats cb;
    cb.attack_power    = stats.attack_power;
    cb.defense         = stats.defense;
    cb.attack_range    = stats.size * 1.8f;
    cb.attack_cooldown = 1.5f;
    m_world->add<CombatStats>(child, cb);

    // Birth glow: offspring flash white briefly
    m_world->add<ColorOverride>(child, {1.0f, 1.0f, 1.0f, 0.8f});

    // Register in spatial grid
    if (m_grid) m_grid->insert(child.index, spawn_pos);

    SHAPE_LOG_TRACE("Reproduction", "Offspring spawned gen={} species={}",
                    cs.generation, sid);

    return child;
}

} // namespace Shape
