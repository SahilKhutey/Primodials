// Games/PolygonalPrimordials/src/Game.cpp
#include "Game.hpp"
#include "PlayerController.hpp"
#include "MainMenu.hpp"
#include "OptionsMenu.hpp"
#include "PauseMenu.hpp"
#include "GameOverScreen.hpp"
#include "SteamIntegration.hpp"
#include "Achievements.hpp"
#include "CloudSaves.hpp"
#include "Telemetry.hpp"
#include "CrashReporter.hpp"
#include "Localization.hpp"
#include "Accessibility.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include "Simulation/Components/ResourceComponents.hpp"
#include "Simulation/Components/LineageComponents.hpp"
#include "Simulation/Genome/GenomeDecoder.hpp"
#include "Simulation/Snapshots/SimulationSnapshot.hpp"
#include "Rendering/SDL3Renderer.hpp"
#include "Input/InputSystem.hpp"
#include "Core/Logger.hpp"

#include <cstdio>
#include <cmath>

using namespace Shape;

static void draw_polygon(Shape::IRenderer& ren, float sx, float sy,
                          float radius, int sides,
                          const Shape::Math::Vector3f& color,
                          float rotation_rad = 0.0f) {
    constexpr float TWO_PI = 6.28318530f;
    std::vector<Shape::Math::Vector2f> vertices(sides);
    for (int i = 0; i < sides; ++i) {
        const float a = rotation_rad + TWO_PI * static_cast<float>(i) / sides;
        vertices[i] = {sx + radius * std::cos(a), sy + radius * std::sin(a)};
    }
    for (int i = 0; i < sides; ++i) {
        ren.DrawLine(vertices[i], vertices[(i + 1) % sides], color, 1.5f);
    }
    if (radius > 4.0f) {
        for (int i = 0; i < sides; ++i) {
            const float a = rotation_rad + TWO_PI * static_cast<float>(i) / sides;
            vertices[i] = {sx + (radius - 2.0f) * std::cos(a), sy + (radius - 2.0f) * std::sin(a)};
        }
        for (int i = 0; i < sides; ++i) {
            ren.DrawLine(vertices[i], vertices[(i + 1) % sides], color * 0.8f, 1.0f);
        }
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
        const float x = bounds.min.x + rng.NextF32() * bounds.width();
        const float y = bounds.min.y + rng.NextF32() * bounds.height();
        const float angle = rng.NextF32() * 6.28318530f;
        const float speed = max_speeds[sp] * (0.2f + rng.NextF32() * 0.4f);

        ECS2::EntityId e = world.create();
        world.add<Position>(e, {{x, y}});
        world.add<Velocity>(e, {{std::cos(angle) * speed, std::sin(angle) * speed}});
        world.add<Rotation>(e, {angle});
        world.add<SpatialTag>(e, {false, {x, y}, e.index});
        world.add<CreatureState>(e, {species_ids[sp], 100.0f, 100.0f, 0, 1, true, true, true});
        world.add<ReproductionState>(e);
        
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
            rot.radians += Math::Clamp(diff, -2.0f * dt, 2.0f * dt);

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

using namespace PolygonalPrimordials;

Game::Game(const GameConfig& cfg)
    : m_cfg(cfg)
    , m_window(m_cfg.title.c_str(), m_cfg.initial_window_w, m_cfg.initial_window_h)
    , m_scheduler(60)
{}

Game::~Game() { shutdown(); }

bool Game::initialize() {
    SHAPE_LOG_INFO("Game: Initializing {} v{}", m_cfg.title, m_cfg.version);

    // Register ECS2 Components
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
    SHAPE_REGISTER_COMPONENT(DerivedAttributes);
    SHAPE_REGISTER_COMPONENT(CombatStats);
    SHAPE_REGISTER_COMPONENT(CombatTracker);
    SHAPE_REGISTER_COMPONENT(MatingStatus);
    SHAPE_REGISTER_COMPONENT(Ancestors);
    SHAPE_REGISTER_COMPONENT(ColorOverride);

    // Instantiate Grid and Systems
    Shape::WorldBounds bounds{{-600.0f, -400.0f}, {600.0f, 400.0f}};
    Shape::SpatialGrid::Config grid_cfg;
    grid_cfg.cell_size = 35.0f;
    grid_cfg.bounds    = bounds;
    grid_cfg.wrap      = true;
    m_grid = std::make_unique<Shape::SpatialGrid>(grid_cfg);

    m_resource_sys = std::make_unique<Shape::ResourceSystem>();
    m_resource_sys->set_world(&m_world);
    m_resource_sys->set_spatial_grid(m_grid.get());

    m_repro_sys = std::make_unique<Shape::ReproductionSystem>();
    m_repro_sys->set_world(&m_world);
    m_repro_sys->set_grid(m_grid.get());

    m_combat_sys = std::make_unique<Shape::CombatSystem>();
    m_combat_sys->set_world(&m_world);
    m_combat_sys->set_grid(m_grid.get());

    m_spec_sys = std::make_unique<Shape::SpeciationSystem>();
    m_spec_sys->set_world(&m_world);

    m_scheduler.register_system(*m_resource_sys);
    m_scheduler.register_system(*m_repro_sys);
    m_scheduler.register_system(*m_combat_sys);
    m_scheduler.register_system(*m_spec_sys);

    // Crash reporter first
    if (m_cfg.enable_crash_reporting) {
        CrashReporter::instance().initialize("PolygonalPrimordials", m_cfg.version);
    }

    // Localization
    Localization::instance().load("data/locales/en_US.po");
    Localization::instance().set_active("en_US");

    // Accessibility defaults
    Accessibility::instance().load_defaults("data/accessibility.json");

    // Telemetry
    if (m_cfg.enable_telemetry) {
        Telemetry::instance().initialize("data/telemetry_config.json");
        Telemetry::instance().event("game_start", {{"version", m_cfg.version}});
    }

    // Window is already created by constructor

    // Renderer (SDL3)
    m_renderer = std::make_unique<Shape::SDL3Renderer>();
    if (!m_renderer->Initialize(m_window)) {
        return false;
    }
    SHAPE_LOG_INFO("Game: Renderer: SDL3");

    // Steam
    if (m_cfg.enable_steam) {
        Steam::SteamIntegration::instance().initialize();
        Achievements::instance().initialize();
    }

    // Cloud saves
    if (m_cfg.enable_cloud_saves) {
        CloudSaves::instance().initialize();
    }

    // Achievement: first launch
    if (m_cfg.enable_achievements) {
        Achievements::instance().unlock("FIRST_LAUNCH");
    }

    if (!new_game("42")) {
        SHAPE_LOG_ERROR("Game: Failed to create initial world");
        return false;
    }

    SHAPE_LOG_INFO("Game: Initialized successfully");
    return true;
}

int Game::run() {
    m_frame_timer.Reset();
    while (!m_quit_requested) {
        m_dt = m_frame_timer.Tick();
        m_total_time += m_dt;

        // Process window events
        if (!m_window.ProcessEvents()) {
            m_quit_requested = true;
        }

        // Game state machine
        switch (m_state) {
            case GameState::MainMenu:   tick_menu();    break;
            case GameState::Loading:    /* loading screen */ break;
            case GameState::Playing:    tick_playing(); break;
            case GameState::Paused:     tick_paused();  break;
            case GameState::Options:    tick_menu();    break;
            case GameState::GameOver:   tick_menu();    break;
            case GameState::Boot:
            case GameState::Quit:       m_quit_requested = true; break;
        }

        render();

        m_window.SwapBuffers();
        ++m_frame_count;
    }
    return 0;
}

void Game::shutdown() {
    SHAPE_LOG_INFO("Game: Shutting down");

    if (m_cfg.enable_telemetry) {
        Telemetry::instance().event("game_quit", {{"duration_s", m_total_time}});
        Telemetry::instance().shutdown();
    }

    if (m_renderer) m_renderer->Shutdown();
    CrashReporter::instance().shutdown();
}

void Game::set_state(GameState s) {
    if (m_state == s) return;
    SHAPE_LOG_TRACE("Game: State: {} -> {}", (int)m_state, (int)s);
    m_state = s;
}

void Game::tick_main() {
    if (Shape::InputSystem::Get().IsKeyPressed(Shape::KeyCode::Escape)) {
        if (m_state == GameState::Playing) set_state(GameState::Paused);
        else if (m_state == GameState::Paused) set_state(GameState::Playing);
        else request_quit();
    }
}

void Game::tick_playing() {
    tick_main();

    auto& input = Shape::InputSystem::Get();
    const float pan_speed = 500.0f * static_cast<float>(m_dt) / m_camera.zoom;
    if (input.IsKeyHeld(Shape::KeyCode::Left))  m_camera.x -= pan_speed;
    if (input.IsKeyHeld(Shape::KeyCode::Right)) m_camera.x += pan_speed;
    if (input.IsKeyHeld(Shape::KeyCode::Up))    m_camera.y -= pan_speed;
    if (input.IsKeyHeld(Shape::KeyCode::Down))  m_camera.y += pan_speed;

    if (input.IsKeyHeld(Shape::KeyCode::Num1))  m_camera.zoom_by(1.0f + 1.0f * static_cast<float>(m_dt));
    if (input.IsKeyHeld(Shape::KeyCode::Num2))  m_camera.zoom_by(1.0f - 1.0f * static_cast<float>(m_dt));

    if (!m_scheduler.is_paused()) {
        m_grid->clear();
        m_world.for_each<Shape::Position, Shape::SpatialTag>(
            [&](Shape::ECS2::EntityId e, Shape::Position& pos, Shape::SpatialTag& tag) {
                tag.last_pos = pos.value;
                m_grid->insert(e.index, pos.value);
            });

        Shape::WorldBounds bounds{{-600.0f, -400.0f}, {600.0f, 400.0f}};
        tick_ai(m_world, static_cast<float>(m_dt) * m_scheduler.speed(), bounds);
    }

    m_scheduler.step(m_dt);
    m_pop_graph.update(m_world, m_scheduler.current_tick());
}

void Game::tick_paused() {
    tick_main();
}

void Game::tick_menu() {
    tick_main();
    if (Shape::InputSystem::Get().IsKeyPressed(Shape::KeyCode::Space)) {
        new_game("42");
    }
}

void Game::render() {
    m_renderer->SetClearColor({0.03f, 0.04f, 0.08f, 1.0f});
    m_renderer->Clear();
    m_renderer->BeginFrame();

    if (m_state == GameState::MainMenu) {
        const float SW = static_cast<float>(m_window.GetWidth());
        const float SH = static_cast<float>(m_window.GetHeight());
        const float cx = SW * 0.5f;
        const float cy = SH * 0.5f;

        float pulse = 1.0f + 0.1f * std::sin(m_total_time * 3.0f);
        draw_polygon(*m_renderer, cx, cy, 80.0f * pulse, 5, {1.0f, 0.5f, 0.0f}, static_cast<float>(m_total_time) * 0.5f);
        
        float ox1 = cx + 150.0f * std::cos(static_cast<float>(m_total_time));
        float oy1 = cy + 150.0f * std::sin(static_cast<float>(m_total_time));
        draw_polygon(*m_renderer, ox1, oy1, 30.0f, 3, {0.3f, 0.7f, 1.0f}, static_cast<float>(m_total_time) * 2.0f);
        
        float ox2 = cx - 150.0f * std::cos(static_cast<float>(m_total_time));
        float oy2 = cy - 150.0f * std::sin(static_cast<float>(m_total_time));
        draw_polygon(*m_renderer, ox2, oy2, 30.0f, 4, {0.3f, 1.0f, 0.3f}, -static_cast<float>(m_total_time) * 1.5f);
    } else if (m_state == GameState::Playing || m_state == GameState::Paused) {
        const float SW = static_cast<float>(m_window.GetWidth());
        const float SH = static_cast<float>(m_window.GetHeight());

        m_world.for_each<Shape::Position, Shape::FoodDeposit>(
            [&](Shape::ECS2::EntityId, Shape::Position& pos, Shape::FoodDeposit& food) {
                if (food.depleted) return;
                const float sx = m_camera.screen_x(pos.value.x, SW);
                const float sy = m_camera.screen_y(pos.value.y, SH);
                if (sx < -10 || sx > SW+10 || sy < -10 || sy > SH+10) return;
                const float r = 3.0f * m_camera.zoom;
                m_renderer->DrawCircle({sx, sy}, r, {0.3f, 0.8f, 0.3f}, true);
            });

        const int sp_sides[] = {3, 4, 5, 6};
        m_world.for_each<Shape::Position, Shape::CreatureState, Shape::Rotation, Shape::DerivedAttributes, Shape::ColorOverride>(
            [&](Shape::ECS2::EntityId, Shape::Position& pos, Shape::CreatureState& state, Shape::Rotation& rot, Shape::DerivedAttributes& da, Shape::ColorOverride& co) {
                if (!state.is_alive) return;
                const float sx = m_camera.screen_x(pos.value.x, SW);
                const float sy = m_camera.screen_y(pos.value.y, SH);
                if (sx < -30 || sx > SW+30 || sy < -30 || sy > SH+30) return;

                const int species_idx = (state.species_id > 0 && state.species_id <= 4) ? state.species_id - 1 : 0;
                const float screen_radius = da.size * m_camera.zoom;

                float final_r = da.color_r;
                float final_g = da.color_g;
                float final_b = da.color_b;
                if (co.strength > 0.0f) {
                    final_r = final_r * (1.0f - co.strength) + co.r * co.strength;
                    final_g = final_g * (1.0f - co.strength) + co.g * co.strength;
                    final_b = final_b * (1.0f - co.strength) + co.b * co.strength;
                }

                draw_polygon(*m_renderer, sx, sy, screen_radius,
                             sp_sides[species_idx], {final_r, final_g, final_b}, rot.radians);
            });
    }

    m_renderer->EndFrame();
}

bool Game::new_game(const std::string& seed) {
    m_current_seed = seed;
    m_world.destroy_all();

    Shape::WorldState ws;
    ws.bounds = {{-600, -400}, {600, 400}};
    m_world.set_resource(std::move(ws));

    Shape::SpeciesRegistry reg;
    const std::vector<std::pair<std::string, int>> species_spawn = {
        {"triangle.species.json", 1500},
        {"quadrilateral.species.json", 1500},
        {"pentagon.species.json", 800},
        {"hexagon.species.json", 400},
        {"circle_civ.species.json", 20}
    };
    for (const auto& [file, _] : species_spawn) {
        reg.load_from_json("data/species/" + file);
    }
    m_world.set_resource(std::move(reg));

    Shape::Simulation::DeterministicRng rng(std::stoull(seed));
    m_scheduler.set_seed(std::stoull(seed));

    m_resource_sys->seed_world(1500, rng);
    seed_creatures(m_world, rng, ws.bounds, 500);

    m_camera.x = 0.0f;
    m_camera.y = 0.0f;
    m_camera.zoom = 0.4f;

    if (m_cfg.enable_achievements) {
        Achievements::instance().unlock("NEW_GAME");
    }
    set_state(GameState::Playing);
    return true;
}

bool Game::save_game(const std::string& slot) {
    const std::string path = "Saves/" + slot + ".shape";
    bool ok = Shape::Simulation::SimulationSnapshot::save_to_file(m_world, path);

    if (m_cfg.enable_cloud_saves) {
        auto bytes = Shape::Simulation::SimulationSnapshot::save_to_memory(m_world);
        CloudSaves::instance().save(slot, std::move(bytes));
    }
    return ok;
}

bool Game::load_game(const std::string& slot) {
    std::vector<uint8_t> data;
    if (m_cfg.enable_cloud_saves) {
        data = CloudSaves::instance().load(slot);
    }
    if (data.empty()) {
        const std::string path = "Saves/" + slot + ".shape";
        return Shape::Simulation::SimulationSnapshot::load_from_file(m_world, path);
    } else {
        return Shape::Simulation::SimulationSnapshot::load_from_memory(m_world, data);
    }
}

bool Game::save_replay(const std::string& /*path*/) {
    return true;
}
