/// @file main.cpp — shape_demo_simulation
/// First running artificial life simulation for Polygonal Primordials.
///
/// Controls:
///   Space       — Pause / Resume
///   1–5         — Speed (0.25x, 1x, 4x, 16x, 64x)
///   F           — Step one tick (when paused)
///   WASD / Arrows — Pan camera
///   Scroll wheel — Zoom
///   ESC         — Exit
///
/// HUD: tick, pop, species count, sim speed, FPS

#include "Core/Logger.hpp"
#include "Core/Platform.hpp"
#include "Simulation/SimulationScheduler.hpp"
#include "Simulation/SimulationState.hpp"
#include "Simulation/Spatial/SpatialGrid.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include "Simulation/Components/ResourceComponents.hpp"
#include "Simulation/Resources/ResourceSystem.hpp"
#include "Simulation/Evolution/EvolutionSystem.hpp"
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

namespace Shape::Demo {

// ─── Register all simulation components ───────────────────────────────────────

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
}

// ─── Camera ───────────────────────────────────────────────────────────────────

struct Camera {
    float x      = 0.0f;
    float y      = 0.0f;
    float zoom   = 1.0f;

    void pan(float dx, float dy) noexcept { x += dx / zoom; y += dy / zoom; }
    void zoom_by(float factor)   noexcept { zoom = Math::clamp(zoom * factor, 0.05f, 20.0f); }

    // World → screen
    float screen_x(float wx, float sw) const noexcept { return (wx - x) * zoom + sw * 0.5f; }
    float screen_y(float wy, float sh) const noexcept { return (wy - y) * zoom + sh * 0.5f; }
};

// ─── Minimal SDL3 polygon renderer ───────────────────────────────────────────

#if HAVE_SDL3

/// Draw a filled regular polygon centred at (sx, sy) in screen space.
static void draw_polygon(SDL_Renderer* ren, float sx, float sy,
                          float radius, int sides,
                          Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                          float rotation_rad = 0.0f) {
    constexpr float TWO_PI = 6.28318530f;
    std::vector<SDL_FPoint> pts;
    pts.reserve(sides + 2);
    pts.push_back({sx, sy});
    for (int i = 0; i <= sides; ++i) {
        const float angle = rotation_rad + TWO_PI * static_cast<float>(i) / static_cast<float>(sides);
        pts.push_back({sx + radius * std::cos(angle),
                       sy + radius * std::sin(angle)});
    }
    SDL_SetRenderDrawColor(ren, r, g, b, a);
    // SDL3 has no filled polygon — use fan triangles via lines (wireframe).
    // For filled look, draw concentric shrinking polygons.
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
        if (ring >= 3) break; // Limit to outer rings for performance
    }
}

// ─── Spawn initial population ──────────────────────────────────────────────

static void seed_creatures(ECS2::World2& world,
                            DeterministicRng& rng,
                            const WorldBounds& bounds,
                            u32 count) {
    // 4 species, roughly equal split
    const SpeciesId species_ids[] = {1, 2, 3, 4}; // Triangle/Quad/Pentagon/Hexagon
    const int sides[] = {3, 4, 5, 6};
    const float radii[] = {6.0f, 9.0f, 12.0f, 15.0f};
    const float max_speeds[] = {90.0f, 55.0f, 40.0f, 28.0f};
    const float r_cols[] = {1.0f, 0.2f, 0.25f, 1.0f};
    const float g_cols[] = {0.35f, 0.7f, 0.45f, 0.85f};
    const float b_cols[] = {0.15f, 0.3f, 0.95f, 0.1f};

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
        world.add<CreatureState>(e, {species_ids[sp], 100.0f, 100.0f, 0, 0, true, false, true});
        world.add<ReproductionState>(e);
        world.add<CreatureGenome>(e);
        world.add<AIState>(e, {CreatureAction::Wander, ~u32{0}, 0.0f, angle});
        world.add<SpeciesIdComponent>(e, {species_ids[sp]});
    }

    std::printf("[Demo] Spawned %u creatures\n", count);
}

// ─── AI tick (wander + seek food) ────────────────────────────────────────────

static void tick_ai(ECS2::World2& world, float dt, const WorldBounds& bounds) {
    world.for_each<Position, Velocity, Rotation, CreatureState, AIState>(
        [&](Position& pos, Velocity& vel, Rotation& rot,
            CreatureState& state, AIState& ai) {
            if (!state.is_alive) return;

            // Simple wander: gradually rotate toward wander_angle
            const float target_angle = ai.wander_angle;
            float diff = target_angle - rot.radians;
            while (diff >  3.14159f) diff -= 6.28318f;
            while (diff < -3.14159f) diff += 6.28318f;
            rot.radians += Math::clamp(diff, -2.0f * dt, 2.0f * dt);

            // Determine base speed from energy
            float base_speed = 50.0f * (0.5f + state.energy / 200.0f);
            vel.value = {std::cos(rot.radians) * base_speed,
                         std::sin(rot.radians) * base_speed};

            // Integrate position
            pos.value.x += vel.value.x * dt;
            pos.value.y += vel.value.y * dt;

            // Toroidal wrap
            bounds.wrap(pos.value);

            // Energy drain (simplified — BiologySystem does proper drain)
            state.age_ticks++;
            state.energy -= 0.5f * dt * 60.0f; // ~0.5 per tick
            if (state.energy < 0.0f) { state.energy = 0.0f; state.is_alive = false; }

            // Periodically change wander direction
            ai.decision_timer -= dt;
            if (ai.decision_timer <= 0.0f) {
                // Use a simple deterministic approach based on age
                const float hash = static_cast<float>((state.age_ticks * 6364136223846793005ULL + 1442695040888963407ULL) & 0xFFFF) / 65536.0f;
                ai.wander_angle = hash * 6.28318530f;
                ai.decision_timer = 0.5f + hash * 1.5f;
            }
        });
}

// ─── main ─────────────────────────────────────────────────────────────────────

int run() {
    std::printf("=== Polygonal Primordials — Simulation Demo ===\n");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Polygonal Primordials — Simulation", 1280, 720, 0);
    if (!window) {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // ── World setup ────────────────────────────────────────────────────────
    register_components();

    ECS2::World2 world;

    // World state resource
    WorldState ws;
    ws.bounds.min = {-800.0f, -500.0f};
    ws.bounds.max = { 800.0f,  500.0f};
    ws.carrying_capacity = 5000;
    ws.wrap_world = true;
    world.set_resource(ws);
    world.set_resource(ResourceMapState{0, 800, 0.05f, 10.0f, 80.0f});

    // Spatial grid
    SpatialGrid::Config grid_cfg;
    grid_cfg.cell_size = 25.0f;
    grid_cfg.bounds    = ws.bounds;
    grid_cfg.wrap      = true;
    SpatialGrid grid(grid_cfg);

    // Scheduler + RNG
    SimulationScheduler scheduler(60);
    scheduler.set_seed(42);

    // Systems
    ResourceSystem resource_sys;
    resource_sys.set_world(&world);
    resource_sys.set_spatial_grid(&grid);

    EvolutionSystem evolution_sys;
    evolution_sys.set_world(&world);

    scheduler.register_system(resource_sys);
    scheduler.register_system(evolution_sys);

    // Seed world
    resource_sys.seed_world(600, scheduler.rng());
    seed_creatures(world, scheduler.rng(), ws.bounds, 1000);

    std::printf("[Demo] World seeded: %zu entities\n", world.entity_count());

    // ── Camera ─────────────────────────────────────────────────────────────
    Camera cam;
    cam.zoom = 0.7f;

    // ── Speed presets ──────────────────────────────────────────────────────
    const float speeds[] = {0.25f, 1.0f, 4.0f, 16.0f, 64.0f};
    int speed_preset = 1;
    scheduler.set_speed(speeds[speed_preset]);

    // ── Main loop ──────────────────────────────────────────────────────────
    bool running = true;
    u64  last_time = SDL_GetTicks();
    u64  frame     = 0;

    while (running) {
        const u64  now      = SDL_GetTicks();
        const float real_dt = static_cast<float>(now - last_time) / 1000.0f;
        last_time = now;
        ++frame;

        // ── Events ────────────────────────────────────────────────────────
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT) running = false;
            if (ev.type == SDL_EVENT_KEY_DOWN) {
                switch (ev.key.scancode) {
                    case SDL_SCANCODE_ESCAPE: running = false; break;
                    case SDL_SCANCODE_SPACE:  scheduler.toggle_pause(); break;
                    case SDL_SCANCODE_F:      if (scheduler.is_paused()) scheduler.step_once(); break;
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

        // Camera pan (keys held)
        const bool* ks = SDL_GetKeyboardState(nullptr);
        const float pan_speed = 300.0f * real_dt / cam.zoom;
        if (ks[SDL_SCANCODE_A] || ks[SDL_SCANCODE_LEFT])  cam.x -= pan_speed;
        if (ks[SDL_SCANCODE_D] || ks[SDL_SCANCODE_RIGHT]) cam.x += pan_speed;
        if (ks[SDL_SCANCODE_W] || ks[SDL_SCANCODE_UP])    cam.y -= pan_speed;
        if (ks[SDL_SCANCODE_S] || ks[SDL_SCANCODE_DOWN])  cam.y += pan_speed;

        // ── Simulation ────────────────────────────────────────────────────
        scheduler.step(static_cast<double>(real_dt));
        tick_ai(world, real_dt * scheduler.speed(), ws.bounds);

        // ── Render ────────────────────────────────────────────────────────
        // Background — deep space / water look
        SDL_SetRenderDrawColor(renderer, 8, 10, 22, 255);
        SDL_RenderClear(renderer);

        const float SW = 1280.0f, SH = 720.0f;

        // Draw grid lines (world boundary)
        SDL_SetRenderDrawColor(renderer, 30, 35, 60, 255);
        const float cx0 = cam.screen_x(ws.bounds.min.x, SW);
        const float cy0 = cam.screen_y(ws.bounds.min.y, SH);
        const float cx1 = cam.screen_x(ws.bounds.max.x, SW);
        const float cy1 = cam.screen_y(ws.bounds.max.y, SH);
        SDL_RenderLine(renderer, cx0, cy0, cx1, cy0);
        SDL_RenderLine(renderer, cx1, cy0, cx1, cy1);
        SDL_RenderLine(renderer, cx1, cy1, cx0, cy1);
        SDL_RenderLine(renderer, cx0, cy1, cx0, cy0);

        // Draw food deposits
        world.for_each<Position, FoodDeposit>(
            [&](Position& pos, FoodDeposit& food) {
                if (food.depleted) return;
                const float sx = cam.screen_x(pos.value.x, SW);
                const float sy = cam.screen_y(pos.value.y, SH);
                if (sx < -10 || sx > SW+10 || sy < -10 || sy > SH+10) return;
                const float r = 3.0f * cam.zoom;
                const float alpha = food.energy / food.max_energy;
                SDL_SetRenderDrawColor(renderer, 80, 220, 80,
                                       static_cast<Uint8>(180 * alpha));
                SDL_FRect rect{sx - r, sy - r, r*2, r*2};
                SDL_RenderFillRect(renderer, &rect);
            });

        // Species config for rendering
        const int   sp_sides[]  = {3, 4, 5, 6};
        const float sp_radii[]  = {6.0f, 9.0f, 12.0f, 15.0f};
        const Uint8 sp_r[]      = {255, 50,  64,  255};
        const Uint8 sp_g[]      = {89,  178, 115, 217};
        const Uint8 sp_b[]      = {38,  76,  242, 25};

        // Draw creatures
        world.for_each<Position, CreatureState, Rotation, SpeciesIdComponent>(
            [&](Position& pos, CreatureState& state, Rotation& rot, SpeciesIdComponent& sp) {
                if (!state.is_alive) return;
                const float sx = cam.screen_x(pos.value.x, SW);
                const float sy = cam.screen_y(pos.value.y, SH);
                if (sx < -30 || sx > SW+30 || sy < -30 || sy > SH+30) return;

                const int species_idx = (sp.id > 0 && sp.id <= 4) ? sp.id - 1 : 0;
                const float screen_radius = sp_radii[species_idx] * cam.zoom;
                if (screen_radius < 1.5f) {
                    // Too small to draw polygon — draw a point
                    SDL_SetRenderDrawColor(renderer, sp_r[species_idx],
                                           sp_g[species_idx], sp_b[species_idx], 255);
                    SDL_RenderPoint(renderer, sx, sy);
                    return;
                }

                // Energy pulse: brighter when energy > 80%
                const float pulse = Math::clamp(state.energy / 100.0f, 0.3f, 1.0f);
                draw_polygon(renderer, sx, sy, screen_radius,
                             sp_sides[species_idx],
                             static_cast<Uint8>(sp_r[species_idx] * pulse),
                             static_cast<Uint8>(sp_g[species_idx] * pulse),
                             static_cast<Uint8>(sp_b[species_idx] * pulse),
                             230, rot.radians);
            });

        // ── HUD ───────────────────────────────────────────────────────────
        // Simple text via filled rectangles (no font loading — SDL3 minimal)
        const auto& stats = scheduler.stats();
        const usize pop   = world.entity_count();
        char hud[256];
        std::snprintf(hud, sizeof(hud),
            "Tick: %llu | Pop: %zu | SimHz: %llu | Speed: %.2fx | %s",
            (unsigned long long)stats.total_ticks, pop,
            (unsigned long long)stats.sim_rate_hz,
            static_cast<double>(scheduler.speed()),
            scheduler.is_paused() ? "PAUSED" : "RUNNING");
        (void)hud; // Printed to terminal every 120 frames

        if (frame % 120 == 0) {
            std::printf("[t=%llu] pop=%zu | simHz=%llu | speed=%.1fx | chunks=%zu\n",
                (unsigned long long)stats.total_ticks,
                pop,
                (unsigned long long)stats.sim_rate_hz,
                static_cast<double>(scheduler.speed()),
                world.total_chunk_count());
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::printf("[Demo] Exited cleanly after %llu ticks.\n",
                (unsigned long long)scheduler.stats().total_ticks);
    return 0;
}

#else

int run() {
    std::fprintf(stderr, "SDL3 not available — simulation demo requires SDL3.\n");
    return 1;
}

#endif

} // namespace Shape::Demo

int main() {
    return Shape::Demo::run();
}
