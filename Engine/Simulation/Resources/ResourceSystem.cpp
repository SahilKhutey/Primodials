#include "Simulation/Resources/ResourceSystem.hpp"
#include "Simulation/SimulationState.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include "Core/Logger.hpp"
#include <algorithm>

namespace Shape {

// ─── seed_world ───────────────────────────────────────────────────────────────

void ResourceSystem::seed_world(u32 count, DeterministicRng& rng) {
    if (!m_world) return;
    for (u32 i = 0; i < count; ++i) {
        const Math::Vector2f pos = random_world_pos(rng);
        const float energy       = 20.0f + rng.NextF32() * 80.0f;

        ECS2::EntityId food = m_world->create();
        m_world->add<Position>(food, {pos});
        m_world->add<FoodDeposit>(food, {energy, energy, 0.15f, false});
        m_world->add<FoodTag>(food);
        m_world->add<SpatialTag>(food);

        if (m_grid) m_grid->insert(food.index, pos);
    }
    SHAPE_LOG_INFO("Resources", "Seeded {} food deposits", count);
}

// ─── update ───────────────────────────────────────────────────────────────────

void ResourceSystem::update(const TickContext& ctx) {
    if (!m_world) return;
    regenerate_food(ctx);
    respawn_food(ctx);
    creature_eat(ctx);
}

// ─── regenerate_food ──────────────────────────────────────────────────────────

void ResourceSystem::regenerate_food(const TickContext& ctx) {
    float sunlight = 0.5f;
    auto& ws = m_world->resource<WorldState>();
    if (ws.bounds.width() > 0.0f) {
        sunlight = ws.environment.sunlight();
    }

    m_world->for_each<FoodDeposit>([&](FoodDeposit& food) {
        if (food.depleted) return;
        food.energy += food.regen_rate * sunlight * static_cast<float>(ctx.fixed_delta);
        if (food.energy > food.max_energy) food.energy = food.max_energy;
    });
}

// ─── respawn_food ─────────────────────────────────────────────────────────────

void ResourceSystem::respawn_food(const TickContext& ctx) {
    usize food_count = 0;
    m_world->for_each<FoodTag>([&](FoodTag&) { ++food_count; });

    u32 target = 500;
    auto& rms = m_world->resource<ResourceMapState>();
    if (rms.target_food_deposits > 0) target = rms.target_food_deposits;

    if (food_count >= target) return;

    auto& rng = const_cast<DeterministicRng&>(ctx.rng);
    if (rng.NextF32() < m_respawn_probability) {
        const Math::Vector2f pos = random_world_pos(rng);
        const float energy       = 15.0f + rng.NextF32() * 60.0f;

        ECS2::EntityId food = m_world->create();
        m_world->add<Position>(food, {pos});
        m_world->add<FoodDeposit>(food, {energy, energy, 0.15f, false});
        m_world->add<FoodTag>(food);
        m_world->add<SpatialTag>(food);

        if (m_grid) m_grid->insert(food.index, pos);
    }
}

// ─── creature_eat ─────────────────────────────────────────────────────────────

void ResourceSystem::creature_eat(const TickContext& ctx) {
    if (!m_grid) return;

    std::vector<u32> nearby;
    nearby.reserve(32);

    m_world->for_each<Position, CreatureState, SpatialTag>(
        [&](ECS2::EntityId /*e*/, Position& pos, CreatureState& state, SpatialTag&) {
            if (!state.is_alive) return;
            if (!state.wants_food) return;
            if (state.energy >= 80.0f) { state.wants_food = false; return; }

            nearby.clear();
            m_grid->query_circle(pos.value, m_eat_radius, nearby);

            for (u32 food_idx : nearby) {
                const ECS2::EntityId food_e{food_idx, 0};
                if (!m_world->valid(food_e)) continue;
                if (!m_world->has<FoodDeposit>(food_e)) continue;

                auto& deposit = m_world->get<FoodDeposit>(food_e);
                if (deposit.depleted || deposit.energy <= 0.0f) continue;

                const float consume = std::min(deposit.energy, m_energy_per_eat);
                const float gained  = consume * 0.7f;
                deposit.energy -= consume;
                state.energy    = std::clamp(state.energy + gained, 0.0f, 100.0f);
                state.wants_food = (state.energy < 60.0f);

                if (deposit.energy <= 0.0f) deposit.depleted = true;
                break;
            }
        });
    (void)ctx;
}

// ─── random_world_pos ─────────────────────────────────────────────────────────

Math::Vector2f ResourceSystem::random_world_pos(DeterministicRng& rng) const noexcept {
    WorldBounds bounds;
    if (m_world) bounds = m_world->resource<WorldState>().bounds;
    const float x = bounds.min.x + rng.NextF32() * bounds.width();
    const float y = bounds.min.y + rng.NextF32() * bounds.height();
    return {x, y};
}

} // namespace Shape
