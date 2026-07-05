// Games/PolygonalPrimordials/src/Game.h
// The main game class — top-level orchestrator
#pragma once

#include "Shape/Core/Time.h"
#include "Shape/Platform/Window.h"
#include "Shape/Rendering/IRenderer.h"
#include "Shape/ECS/World.h"
#include "Shape/Simulation/SimulationScheduler.h"
#include "Shape/Simulation/Species/SpeciesRegistry.h"
#include "Shape/Simulation/Spatial/SpatialGrid.h"
#include "Shape/Simulation/Analytics/PopulationGraph.h"
#include "Shape/Simulation/Components/WorldComponents.h"

#include <memory>
#include <string>
#include <vector>

namespace PolygonalPrimordials {

    enum class GameState : uint8_t {
        Boot,
        MainMenu,
        Loading,
        Playing,
        Paused,
        Options,
        GameOver,
        Quit
    };

    struct GameConfig {
        std::string title = "Polygonal Primordials";
        std::string version = "1.0.0";
        bool enable_steam = true;
        bool enable_telemetry = true;
        bool enable_crash_reporting = true;
        bool enable_cloud_saves = true;
        bool enable_workshop = true;
        bool enable_achievements = true;
        bool headless = false;
        int initial_window_w = 1600;
        int initial_window_h = 1000;
    };

    class Game {
    public:
        explicit Game(const GameConfig& cfg = {});
        ~Game();

        bool initialize();
        int run();
        void shutdown();

        // State
        void set_state(GameState s);
        GameState state() const { return m_state; }
        void request_quit() { m_quit_requested = true; }
        bool is_running() const { return !m_quit_requested; }

        // Access
        Shape::ECS::World& world() { return m_world; }
        const Shape::ECS::World& world() const { return m_world; }
        Shape::Simulation::SimulationScheduler& scheduler() { return m_scheduler; }
        Shape::Platform::Window& window() { return m_window; }
        Shape::Rendering::IRenderer* renderer() const { return m_renderer.get(); }
        const GameConfig& config() const { return m_cfg; }

        // Save/load
        bool new_game(const std::string& world_seed);
        bool save_game(const std::string& slot = "quicksave");
        bool load_game(const std::string& slot = "quicksave");
        bool save_replay(const std::string& path);

    private:
        void tick_main();
        void tick_playing();
        void tick_paused();
        void tick_menu();
        void render();

        void on_window_close();
        void on_key(int key, int scancode, int action, int mods);

        GameConfig m_cfg;
        GameState m_state = GameState::Boot;
        bool m_quit_requested = false;
        double m_total_time = 0;
        uint64_t m_frame_count = 0;

        // Engine subsystems
        Shape::Platform::Window m_window;
        std::unique_ptr<Shape::Rendering::IRenderer> m_renderer;
        Shape::ECS::World m_world;
        Shape::Simulation::SimulationScheduler m_scheduler;
        Shape::Simulation::Analytics::PopulationGraph m_pop_graph;

        // Game state
        std::string m_current_seed = "2024";
        std::string m_current_slot = "quicksave";
    };

} // namespace PolygonalPrimordials
