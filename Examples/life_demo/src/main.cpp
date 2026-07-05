/// @file main.cpp — shape_demo_life
/// Complete Phase D artificial life sandbox with reproduction and combat.

#include "Core/Logger.hpp"
#include "Core/Platform.hpp"
#include "Simulation/SimulationScheduler.hpp"
#include "Simulation/SimulationState.hpp"
#include "Simulation/Spatial/SpatialGrid.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include "Simulation/Components/ResourceComponents.hpp"
#include "Simulation/Components/LineageComponents.hpp"
#include "Simulation/Resources/ResourceSystem.hpp"
#include "Simulation/Evolution/EvolutionSystem.hpp"
#include "Simulation/Evolution/SpeciationSystem.hpp"
#include "Simulation/Reproduction/ReproductionSystem.hpp"
#include "Simulation/Combat/CombatSystem.hpp"
#include "Simulation/Snapshots/SimulationSnapshot.hpp"
#include "Simulation/Analytics/PopulationGraph.hpp"
#include "Simulation/Analytics/SimulationMetrics.hpp"
#include "ECS2/World2.hpp"
#include "ECS2/ComponentRegistry.hpp"
#include "ECS/SpeciesSchema.hpp"
#include "Math/Vec2.hpp"

#if __has_include(<SDL3/SDL.h>)
#include <SDL3/SDL.h>
#define HAVE_SDL3 1
#else
#define HAVE_SDL3 0
#endif

#include <cstdio>
#include <cmath>
#include <vector>
#include <array>
#include <string>
#include <deque>

namespace Shape::Demo {

void register_components() {
    SHAPE_REGISTER_COMPONENT(Position);
    SHAPE_REGISTER_COMPONENT(Velocity);
    SHAPE_REGISTER_COMPONENT(Rotation);
    SHAPE_REGISTER_COMPONENT(SpatialTag);
    SHAPE_REGISTER_COMPONENT(CreatureState);
    SHAPE_REGISTER_COMPONENT(ReproductionState);
    SHAPE_REGISTER_COMPONENT(CreatureGenome);
    SHAPE_REGISTER_COMPONENT(FitnessScore);
    SHAPE_REGISTER_COMPONENT(AIState);
    SHAPE_REGISTER_COMPONENT(FoodDeposit);
    SHAPE_REGISTER_COMPONENT(FoodTag);
    SHAPE_REGISTER_COMPONENT(SpeciesIdComponent);
    // Phase D components
    SHAPE_REGISTER_COMPONENT(DerivedAttributes);
    SHAPE_REGISTER_COMPONENT(CombatStats);
    SHAPE_REGISTER_COMPONENT(CombatTracker);
    SHAPE_REGISTER_COMPONENT(MatingStatus);
    SHAPE_REGISTER_COMPONENT(Ancestors);
    SHAPE_REGISTER_COMPONENT(ColorOverride);
}

struct Camera {
    float x      = 0.0f;
    float y      = 0.0f;
    float zoom   = 1.0f;

    void pan(float dx, float dy) noexcept { x += dx / zoom; y += dy / zoom; }
    void zoom_by(float factor)   noexcept { zoom = Math::clamp(zoom * factor, 0.05f, 20.0f); }

    float screen_x(float wx, float sw) const noexcept { return (wx - x) * zoom + sw * 0.5f; }
    float screen_y(float wy, float sh) const noexcept { return (wy - y) * zoom + sh * 0.5f; }
};

#if HAVE_SDL3

static void draw_polygon(SDL_Renderer* ren, float sx, float sy,
                          float radius, int sides,
                          Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                          float rotation_rad = 0.0f) {
    constexpr float TWO_PI = 6.28318530f;
    for (int ring = 0; ring < static_cast<int>(radius); ++ring) {
        const float rr = radius - static_cast<float>(ring);
        const float fade = static_cast<float>(ring) / radius;
        const Uint8 ar = static_cast<Uint8>(r * (1.0f - fade * 0.5f));
        const Uint8 ag = static_cast<Uint8>(g * (1.0f - fade * 0.5f));
        const Uint8 ab = static_cast<Uint8>(b * (1.0f - fade * 0.5f));
        SDL_SetRenderDrawColor(ren, ar, ag, ab, a);
        for (int i = 0; i < sides; ++i) {
            const float a0 = rotation_rad + TWO_PI * static_cast<float>(i)   / sides;
            const float a1 = rotation_rad + TWO_PI * static_cast<float>(i+1) / sides;
            SDL_RenderLine(ren,
                sx + rr * std::cos(a0), sy + rr * std::sin(a0),
                sx + rr * std::cos(a1), sy + rr * std::sin(a1));
        }
        if (ring >= 3) break;
    }
}

static void seed_creatures(ECS2::World2& world,
                            Simulation::DeterministicRng& rng,
                            const WorldBounds& bounds,
                            u32 count) {
    const SpeciesId species_ids[] = {1, 2, 3, 4};
    const float max_speeds[] = {90.0f, 55.0f, 40.0f, 28.0f};

    for (u32 i = 0; i < count; ++i) {
        const u32 sp = i % 4;
        const float x = bounds.min.x + rng.NextFloat() * bounds.width();
        const float y = bounds.min.y + rng.NextFloat() * bounds.height();
        const float angle = rng.NextFloat() * 6.28318530f;
        const float speed = max_speeds[sp] * (0.2f + rng.NextFloat() * 0.4f);

        ECS2::EntityId e = world.create();
        world.add<Position>(e, {{x, y}});
        world.add<Velocity>(e, {{std::cos(angle) * speed, std::sin(angle) * speed}});
        world.add<Rotation>(e, {angle});
        world.add<SpatialTag>(e, {false, {x, y}, e.index});
        world.add<CreatureState>(e, {species_ids[sp], 100.0f, 100.0f, 0, 1, true, true, true});
        world.add<ReproductionState>(e);
        
        // Base genome
        CreatureGenome cg;
        for (u8 g = 0; g < 16; ++g) cg.genes[g] = 0.5f; 
        world.add<CreatureGenome>(e, cg);
        
        world.add<AIState>(e, {CreatureAction::Wander, ~u32{0}, 0.0f, angle});
        world.add<SpeciesIdComponent>(e, {species_ids[sp]});
        world.add<MatingStatus>(e);
        world.add<CombatTracker>(e);
        world.add<Ancestors>(e, {ECS2::EntityId::invalid(), ECS2::EntityId::invalid(), {}, {}, {}, {}, 1});
        world.add<ColorOverride>(e);

        DerivedAttributes da;
        da.max_health = 100.0f;
        da.max_energy = 100.0f;
        da.max_speed  = max_speeds[sp];
        da.size = 6.0f + 3.0f * sp;
        da.mass = 1.0f + 1.0f * sp;
        da.color_r = sp == 0 ? 1.0f : (sp == 2 ? 0.2f : 1.0f);
        da.color_g = sp == 1 ? 1.0f : 0.4f;
        da.color_b = sp == 2 ? 1.0f : 0.2f;
        da.iq = 50.0f;
        da.aggression = 0.5f;
        world.add<DerivedAttributes>(e, da);

        CombatStats cb;
        cb.attack_power = 10.0f + 2.0f * sp;
        cb.defense = 5.0f + 1.0f * sp;
        cb.attack_range = da.size * 1.5f;
        world.add<CombatStats>(e, cb);
    }
}

static void tick_ai(ECS2::World2& world, float dt, const WorldBounds& bounds) {
    world.for_each<Position, Velocity, Rotation, CreatureState, AIState, DerivedAttributes>(
        [&](Position& pos, Velocity& vel, Rotation& rot,
            CreatureState& state, AIState& ai, DerivedAttributes& da) {
            if (!state.is_alive) return;

            const float target_angle = ai.wander_angle;
            float diff = target_angle - rot.radians;
            while (diff >  3.14159f) diff -= 6.28318f;
            while (diff < -3.14159f) diff += 6.28318f;
            rot.radians += Math::clamp(diff, -2.0f * dt, 2.0f * dt);

            float base_speed = da.max_speed * (0.5f + state.energy / (da.max_energy * 2.0f));
            vel.value = {std::cos(rot.radians) * base_speed,
                         std::sin(rot.radians) * base_speed};

            pos.value.x += vel.value.x * dt;
            pos.value.y += vel.value.y * dt;
            bounds.wrap(pos.value);

            state.age_ticks++;
            state.energy -= da.metabolism * dt * 60.0f;
            if (state.energy < 0.0f) { state.energy = 0.0f; state.is_alive = false; }

            ai.decision_timer -= dt;
            if (ai.decision_timer <= 0.0f) {
                const float hash = static_cast<float>((state.age_ticks * 6364136223846793005ULL + 1442695040888963407ULL) & 0xFFFF) / 65536.0f;
                ai.wander_angle = hash * 6.28318530f;
                ai.decision_timer = 0.5f + hash * 1.5f;
            }
        });
}

int run() {
    if (!SDL_Init(SDL_INIT_VIDEO)) return 1;
    SDL_Window* window = SDL_CreateWindow("Polygonal Primordials — Phase D Life Demo", 1280, 720, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    register_components();
    ECS2::World2 world;
    WorldState ws;
    ws.bounds.min = {-1500.0f, -1000.0f};
    ws.bounds.max = { 1500.0f,  1000.0f};
    ws.carrying_capacity = 20000;
    ws.wrap_world = true;
    world.set_resource(ws);

    SpatialGrid::Config grid_cfg;
    grid_cfg.cell_size = 35.0f;
    grid_cfg.bounds    = ws.bounds;
    grid_cfg.wrap      = true;
    SpatialGrid grid(grid_cfg);

    SimulationScheduler scheduler(60);
    scheduler.set_seed(42);

    ResourceSystem resource_sys;
    resource_sys.set_world(&world);
    resource_sys.set_spatial_grid(&grid);

    ReproductionSystem repro_sys;
    repro_sys.set_world(&world);
    repro_sys.set_grid(&grid);

    CombatSystem combat_sys;
    combat_sys.set_world(&world);
    combat_sys.set_grid(&grid);

    SpeciationSystem spec_sys;
    spec_sys.set_world(&world);

    scheduler.register_system(resource_sys);
    scheduler.register_system(repro_sys);
    scheduler.register_system(combat_sys);
    scheduler.register_system(spec_sys);

    resource_sys.seed_world(1500, scheduler.rng());
    seed_creatures(world, scheduler.rng(), ws.bounds, 5000);

    Camera cam;
    cam.zoom = 0.4f;

    const float speeds[] = {0.25f, 1.0f, 4.0f, 16.0f, 64.0f};
    int speed_preset = 1;
    scheduler.set_speed(speeds[speed_preset]);

    Simulation::Analytics::PopulationGraph pop_graph;
    std::deque<std::vector<u8>> rewinds;

    bool running = true;
    u64  last_time = SDL_GetTicks();
    u64  frame     = 0;

    while (running) {
        const u64  now = SDL_GetTicks();
        const float real_dt = static_cast<float>(now - last_time) / 1000.0f;
        last_time = now;
        ++frame;

        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT) running = false;
            if (ev.type == SDL_EVENT_KEY_DOWN) {
                switch (ev.key.scancode) {
                    case SDL_SCANCODE_ESCAPE: running = false; break;
                    case SDL_SCANCODE_SPACE:  scheduler.toggle_pause(); break;
                    case SDL_SCANCODE_S:
                        Simulation::SimulationSnapshot::save_to_file(world, "snapshot.shsnap");
                        std::printf("Saved to snapshot.shsnap\n");
                        break;
                    case SDL_SCANCODE_L:
                        if (Simulation::SimulationSnapshot::load_from_file(world, "snapshot.shsnap")) {
                            std::printf("Loaded from snapshot.shsnap\n");
                        }
                        break;
                    case SDL_SCANCODE_R: // Rewind
                        if (!rewinds.empty()) {
                            Simulation::SimulationSnapshot::load_from_memory(world, rewinds.back());
                            rewinds.pop_back();
                            std::printf("Rewound time by 10s. Snapshots remaining: %zu\n", rewinds.size());
                        }
                        break;
                    case SDL_SCANCODE_1: case SDL_SCANCODE_2: case SDL_SCANCODE_3:
                    case SDL_SCANCODE_4: case SDL_SCANCODE_5:
                        speed_preset = ev.key.scancode - SDL_SCANCODE_1;
                        scheduler.set_speed(speeds[speed_preset]);
                        break;
                    default: break;
                }
            }
            if (ev.type == SDL_EVENT_MOUSE_WHEEL) {
                cam.zoom_by(ev.wheel.y > 0 ? 1.15f : 0.87f);
            }
        }

        const bool* ks = SDL_GetKeyboardState(nullptr);
        const float pan_speed = 500.0f * real_dt / cam.zoom;
        if (ks[SDL_SCANCODE_A] || ks[SDL_SCANCODE_LEFT])  cam.x -= pan_speed;
        if (ks[SDL_SCANCODE_D] || ks[SDL_SCANCODE_RIGHT]) cam.x += pan_speed;
        if (ks[SDL_SCANCODE_W] || ks[SDL_SCANCODE_UP])    cam.y -= pan_speed;
        if (ks[SDL_SCANCODE_S] || ks[SDL_SCANCODE_DOWN])  cam.y += pan_speed;

        // Auto snapshot every 600 ticks (~10 seconds)
        if (scheduler.stats().total_ticks % 600 == 0 && scheduler.stats().total_ticks > 0) {
            rewinds.push_back(Simulation::SimulationSnapshot::save_to_memory(world));
            if (rewinds.size() > 30) rewinds.pop_front();
        }

        scheduler.step(static_cast<double>(real_dt));
        pop_graph.update(world, scheduler.stats().total_ticks);
        tick_ai(world, real_dt * scheduler.speed(), ws.bounds);

        SDL_SetRenderDrawColor(renderer, 8, 10, 22, 255);
        SDL_RenderClear(renderer);

        const float SW = 1280.0f, SH = 720.0f;
        
        // Deposits
        world.for_each<Position, FoodDeposit>(
            [&](Position& pos, FoodDeposit& food) {
                if (food.depleted) return;
                const float sx = cam.screen_x(pos.value.x, SW);
                const float sy = cam.screen_y(pos.value.y, SH);
                if (sx < -10 || sx > SW+10 || sy < -10 || sy > SH+10) return;
                const float r = 3.0f * cam.zoom;
                const float alpha = food.energy / food.max_energy;
                SDL_SetRenderDrawColor(renderer, 80, 220, 80, static_cast<Uint8>(180 * alpha));
                SDL_FRect rect{sx - r, sy - r, r*2, r*2};
                SDL_RenderFillRect(renderer, &rect);
            });

        const int sp_sides[] = {3, 4, 5, 6};

        // Creatures
        world.for_each<Position, CreatureState, Rotation, DerivedAttributes, ColorOverride>(
            [&](Position& pos, CreatureState& state, Rotation& rot, DerivedAttributes& da, ColorOverride& co) {
                if (!state.is_alive) return;
                const float sx = cam.screen_x(pos.value.x, SW);
                const float sy = cam.screen_y(pos.value.y, SH);
                if (sx < -30 || sx > SW+30 || sy < -30 || sy > SH+30) return;

                const int species_idx = (state.species_id > 0 && state.species_id <= 4) ? state.species_id - 1 : 0;
                const float screen_radius = da.size * cam.zoom;
                
                // Color blending
                float final_r = da.color_r;
                float final_g = da.color_g;
                float final_b = da.color_b;
                if (co.strength > 0.0f) {
                    final_r = final_r * (1.0f - co.strength) + co.r * co.strength;
                    final_g = final_g * (1.0f - co.strength) + co.g * co.strength;
                    final_b = final_b * (1.0f - co.strength) + co.b * co.strength;
                }
                
                Uint8 r8 = static_cast<Uint8>(Math::clamp(final_r * 255.0f, 0.0f, 255.0f));
                Uint8 g8 = static_cast<Uint8>(Math::clamp(final_g * 255.0f, 0.0f, 255.0f));
                Uint8 b8 = static_cast<Uint8>(Math::clamp(final_b * 255.0f, 0.0f, 255.0f));

                draw_polygon(renderer, sx, sy, screen_radius,
                             sp_sides[species_idx], r8, g8, b8, 230, rot.radians);
            });

        if (frame % 60 == 0) {
            std::printf("[t=%llu] pop=%zu | simHz=%llu | snapshots=%zu\n",
                (unsigned long long)scheduler.stats().total_ticks,
                world.entity_count(),
                (unsigned long long)scheduler.stats().sim_rate_hz,
                rewinds.size());
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

#else
int run() { return 1; }
#endif
} // namespace Shape::Demo

int main() { return Shape::Demo::run(); }
