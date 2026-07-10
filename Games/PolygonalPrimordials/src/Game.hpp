// Games/PolygonalPrimordials/src/Game.h
// The main game class — top-level orchestrator
#pragma once

#include "Core/Time.hpp"
#include "Platform/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "ECS2/World2.hpp"
#include "Simulation/SimulationScheduler.hpp"
#include "ECS/SpeciesRegistry.hpp"
#include "Simulation/Spatial/SpatialGrid.hpp"
#include "Simulation/Resources/ResourceSystem.hpp"
#include "Simulation/Reproduction/ReproductionSystem.hpp"
#include "Simulation/Combat/CombatSystem.hpp"
#include "Simulation/Evolution/SpeciationSystem.hpp"
#include "Simulation/Analytics/PopulationGraph.hpp"

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
        Shape::ECS2::World2& world() { return m_world; }
        const Shape::ECS2::World2& world() const { return m_world; }
        Shape::SimulationScheduler& scheduler() { return m_scheduler; }
        Shape::Window& window() { return m_window; }
        Shape::IRenderer* renderer() const { return m_renderer.get(); }
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
        double m_dt = 0;
        uint64_t m_frame_count = 0;
        Shape::Time::FrameTimer m_frame_timer;

        // Engine subsystems
        Shape::Window m_window;
        std::unique_ptr<Shape::IRenderer> m_renderer;
        Shape::ECS2::World2 m_world;
        Shape::SimulationScheduler m_scheduler;
        Shape::Simulation::Analytics::PopulationGraph m_pop_graph;

        struct Camera {
            float x      = 0.0f;
            float y      = 0.0f;
            float zoom   = 0.4f;

            void pan(float dx, float dy) noexcept { x += dx / zoom; y += dy / zoom; }
            void zoom_by(float factor)   noexcept { zoom = Shape::Math::Clamp(zoom * factor, 0.05f, 20.0f); }

            float screen_x(float wx, float sw) const noexcept { return (wx - x) * zoom + sw * 0.5f; }
            float screen_y(float wy, float sh) const noexcept { return (wy - y) * zoom + sh * 0.5f; }
        } m_camera;

        std::unique_ptr<Shape::SpatialGrid> m_grid;
        std::unique_ptr<Shape::ResourceSystem> m_resource_sys;
        std::unique_ptr<Shape::ReproductionSystem> m_repro_sys;
        std::unique_ptr<Shape::CombatSystem> m_combat_sys;
        std::unique_ptr<Shape::SpeciationSystem> m_spec_sys;

        // Game state
        std::string m_current_seed = "2024";
        std::string m_current_slot = "quicksave";
    };

} // namespace PolygonalPrimordials
