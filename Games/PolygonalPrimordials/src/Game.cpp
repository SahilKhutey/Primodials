// Games/PolygonalPrimordials/src/Game.cpp
#include "Game.h"
#include "PlayerController.h"
#include "MainMenu.h"
#include "OptionsMenu.h"
#include "PauseMenu.h"
#include "GameOverScreen.h"
#include "SteamIntegration.h"
#include "Achievements.h"
#include "CloudSaves.h"
#include "Telemetry.h"
#include "CrashReporter.h"
#include "Localization.h"
#include "Accessibility.h"
#include "Shape/Simulation/Systems/BiologySystem.h"
#include "Shape/Simulation/Systems/ResourceSystem.h"
#include "Shape/Simulation/Systems/ReproductionSystem.h"
#include "Shape/Simulation/Systems/CombatSystem.h"
#include "Shape/Simulation/Systems/EvolutionSystem.h"
#include "Shape/Simulation/Systems/RenderSyncSystem.h"
#include "Shape/Simulation/Systems/AISystem.h"
#include "Shape/Simulation/Systems/SpatialSystem.h"
#include "Shape/Simulation/Systems/TechnologySystem.h"
#include "Shape/Simulation/Systems/DiplomacySystem.h"
#include "Shape/Simulation/Systems/GroupBehaviorSystem.h"
#include "Shape/Simulation/Components/CreatureComponents.h"
#include "Shape/Simulation/Components/ResourceComponents.h"
#include "Shape/Simulation/Components/WorldComponents.h"
#include "Shape/Simulation/Components/LineageComponents.h"
#include "Shape/Simulation/Genome/GenomeDecoder.h"
#include "Shape/Simulation/Snapshots/SimulationSnapshot.h"
#include "Shape/Rendering/EntityRenderer.h"
#include "Shape/Core/Logger.h"
#include "Shape/Random/DeterministicRng.h"
#include "Shape/Rendering/RendererFactory.h"

#include <cstdio>

using namespace PolygonalPrimordials;

Game::Game(const GameConfig& cfg) : m_cfg(cfg) {}

Game::~Game() { shutdown(); }

bool Game::initialize() {
    SHAPE_LOG_INFO("Game", "Initializing {} v{}", m_cfg.title, m_cfg.version);

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

    // Window
    Shape::Platform::WindowDesc wd;
    wd.title = m_cfg.title + " v" + m_cfg.version;
    wd.size = {m_cfg.initial_window_w, m_cfg.initial_window_h};
    wd.vsync = true;
    if (!m_window.create(wd)) {
        SHAPE_LOG_FATAL("Game", "Window creation failed");
        return false;
    }
    Shape::Platform::Input::instance().initialize();

    // Renderer (auto-detect best backend)
    m_renderer = Shape::Rendering::RendererFactory::create_best_renderer(m_window);
    if (!m_renderer) {
        return false;
    }
    SHAPE_LOG_INFO("Game", "Renderer: {}", m_renderer->backend_name());

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

    set_state(GameState::MainMenu);
    SHAPE_LOG_INFO("Game", "Initialized successfully");
    return true;
}

int Game::run() {
    while (!m_quit_requested) {
        const double dt = m_window.delta_time();
        m_total_time += dt;

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

        m_window.swap_buffers();
        ++m_frame_count;
    }
    return 0;
}

void Game::shutdown() {
    SHAPE_LOG_INFO("Game", "Shutting down");

    if (m_cfg.enable_telemetry) {
        Telemetry::instance().event("game_quit", {{"duration_s", m_total_time}});
        Telemetry::instance().shutdown();
    }

    if (m_renderer) m_renderer->shutdown();
    Shape::Platform::Input::instance().shutdown();
    m_window.destroy();
    CrashReporter::instance().shutdown();
}

void Game::set_state(GameState s) {
    if (m_state == s) return;
    SHAPE_LOG_DEBUG("Game", "State: {} -> {}", (int)m_state, (int)s);
    m_state = s;
}

void Game::tick_main() {
    // Common frame work
    Shape::Platform::Input::instance().begin_frame();
    m_window.poll_events();

    if (Shape::Platform::Input::instance().was_key_pressed(Shape::Platform::Key::Escape)) {
        if (m_state == GameState::Playing) set_state(GameState::Paused);
        else if (m_state == GameState::Paused) set_state(GameState::Playing);
        else request_quit();
    }
}

void Game::tick_playing() {
    tick_main();
    m_scheduler.step(m_world, m_window.delta_time());
    m_pop_graph.update(m_world, m_scheduler.current_tick());
}

void Game::tick_paused() {
    tick_main();
}

void Game::tick_menu() {
    tick_main();
    // Menu-specific input handling
}

void Game::render() {
    const auto size = m_window.drawable_size();
    m_renderer->resize(size);
    m_renderer->set_viewport(0, 0, size.x, size.y);
    m_renderer->begin_frame(Shape::Math::Color{0.04f, 0.04f, 0.06f, 1.0f});
    // Game-specific rendering
    m_renderer->end_frame();
    Shape::Platform::Input::instance().end_frame();
}

bool Game::new_game(const std::string& seed) {
    m_current_seed = seed;
    m_world.destroy_all();

    // Setup world
    Shape::Simulation::WorldState ws;
    ws.bounds = {{-600, -400}, {600, 400}};
    ws.diplomacy = std::make_unique<Shape::Simulation::DiplomacyState>();
    Shape::Random::DeterministicRng rng(std::stoull(seed));
    ws.rng = &rng;
    m_world.set_resource(std::move(ws));

    // Load species
    auto reg = std::make_unique<Shape::Simulation::SpeciesRegistry>();
    const std::vector<std::pair<std::string, int>> species_spawn = {
        {"triangle.species.json", 1500},
        {"quadrilateral.species.json", 1500},
        {"pentagon.species.json", 800},
        {"hexagon.species.json", 400},
        {"circle_civ.species.json", 20}
    };
    for (const auto& [file, _] : species_spawn) {
        reg->load_from_json("data/species/" + file);
    }
    Shape::Simulation::SpeciesRegistryResource srr;
    srr.registry = std::move(reg);
    m_world.set_resource(std::move(srr));

    // Achievement: started a new game
    if (m_cfg.enable_achievements) {
        Achievements::instance().unlock("NEW_GAME");
    }
    set_state(GameState::Playing);
    return true;
}

bool Game::save_game(const std::string& slot) {
    Shape::Simulation::SimulationSnapshot snap = Shape::Simulation::SimulationSerializer::capture(m_world, {});
    snap.saved_at_tick = m_scheduler.current_tick();
    snap.saved_at_time = m_scheduler.sim_time();
    const std::string path = "Saves/" + slot + ".shape";
    bool ok = Shape::Simulation::SimulationSerializer::save(snap, path);

    if (m_cfg.enable_cloud_saves) {
        CloudSaves::instance().save(slot, Shape::Simulation::SimulationSerializer::save_to_bytes(snap));
    }
    return ok;
}

bool Game::load_game(const std::string& slot) {
    std::vector<uint8_t> data;
    if (m_cfg.enable_cloud_saves) {
        data = CloudSaves::instance().load(slot);
    }
    if (data.empty()) {
        const auto snap = Shape::Simulation::SimulationSerializer::load("Saves/" + slot + ".shape");
        if (!snap.is_valid()) return false;
        Shape::Simulation::SimulationSerializer::apply(m_world, snap);
    } else {
        const auto snap = Shape::Simulation::SimulationSerializer::load_from_bytes(data);
        if (!snap.is_valid()) return false;
        Shape::Simulation::SimulationSerializer::apply(m_world, snap);
    }
    return true;
}

bool Game::save_replay(const std::string& /*path*/) {
    return true;
}
