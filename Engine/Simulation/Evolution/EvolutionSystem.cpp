#include "Simulation/Evolution/EvolutionSystem.hpp"
#include "Simulation/SimulationState.hpp"
#include "Core/Logger.hpp"
#include <vector>
#include <algorithm>

namespace Shape {

// ─── Fitness function ─────────────────────────────────────────────────────────

float EvolutionSystem::compute_fitness(const CreatureState& state,
                                        const ReproductionState& repro,
                                        const Species& species) noexcept {
    // Age factor: favour middle-age over both juvenile and elderly
    const float lifespan     = species.biology.lifespan;
    const float age_norm     = static_cast<float>(state.age_ticks) / lifespan;
    const float age_factor   = 1.0f - std::abs(age_norm - 0.5f) * 2.0f; // 1 at 50% life

    // Energy factor: direct measure of current survival success
    const float energy_factor = std::clamp(state.energy / species.biology.maxEnergy,
                                             0.0f, 1.0f);

    // Offspring factor: reproductive success
    const float offspring_factor = std::clamp(
        static_cast<float>(repro.offspring_count) / 10.0f, 0.0f, 1.0f);

    // Health factor
    const float health_factor = std::clamp(state.health / species.biology.maxHealth,
                                             0.0f, 1.0f);

    // Weighted combination
    return 0.30f * energy_factor
         + 0.25f * health_factor
         + 0.25f * offspring_factor
         + 0.20f * age_factor;
}

// ─── update ───────────────────────────────────────────────────────────────────

void EvolutionSystem::update(const TickContext& ctx) {
    // Run selection only every m_selection_interval ticks
    if (ctx.tick % m_selection_interval != 0) return;
    if (!m_world) return;

    run_selection(ctx);
}

void EvolutionSystem::run_selection(const TickContext& ctx) {
    if (!m_world) return;

    // Get world bounds / capacity from resource
    // (WorldState may not be set in standalone test contexts)
    u32 carrying_capacity = 10000;
    if (m_world->resource<WorldState>().carrying_capacity > 0) {
        carrying_capacity = m_world->resource<WorldState>().carrying_capacity;
    }

    const usize pop = m_world->entity_count();
    if (pop == 0) return;

    // --- Overpopulation culling ---
    if (pop > carrying_capacity) {
        // Gather fitness scores
        struct Entry { ECS2::EntityId e; float fitness; };
        std::vector<Entry> entries;
        entries.reserve(pop);

        m_world->for_each<CreatureState, ReproductionState>(
            [&](ECS2::EntityId e, CreatureState& cs, ReproductionState& rs) {
                if (!cs.is_alive) return;
                // Use default species for fitness (full lookup deferred to Phase D)
                const float f = 0.5f * (cs.energy / 100.0f) + 0.5f * (cs.health / 100.0f);
                entries.push_back({e, f});
            });

        // Sort ascending by fitness — least fit first
        std::sort(entries.begin(), entries.end(),
                  [](const Entry& a, const Entry& b) { return a.fitness < b.fitness; });

        // Cull until within capacity (max 5% per selection cycle to prevent mass-die-offs)
        const usize max_cull = static_cast<usize>(pop * 0.05f) + 1;
        const usize cull_count = std::min(max_cull, pop - carrying_capacity);

        usize culled = 0;
        for (const auto& entry : entries) {
            if (culled >= cull_count) break;
            if (m_world->valid(entry.e)) {
                m_world->destroy(entry.e);
                ++culled;
            }
        }

        if (culled > 0) {
            SHAPE_LOG_INFO("Evolution", "Tick {} — culled {} (capacity {})",
                           ctx.tick, culled, carrying_capacity);
        }
    }

    // --- Update SimulationStats ---
    auto& world_state = m_world->resource<WorldState>();
    world_state.stats.current_population = m_world->entity_count();
    if (world_state.stats.current_population > world_state.stats.peak_population) {
        world_state.stats.peak_population      = world_state.stats.current_population;
        world_state.stats.peak_population_tick = ctx.tick;
    }
    world_state.tick(ctx.tick);
}

} // namespace Shape
