// Engine/GameView/GameViewWindow.h
#pragma once

#include "ECS/World.hpp"
#include <SDL3/SDL.h>
#include <string>
#include <memory>
#include <functional>

namespace ShapeEngine::GameView {

    enum class GameViewAction {
        None,
        PauseToggle,
        SpeedUp,
        SpeedDown,
        SaveSnapshot,
        LoadSnapshot,
        OpenThemeSelector,
        OpenSettings,
        OpenInspector,
        Close,
        Quit
    };

    struct GameViewColorRGBA {
        Uint8 r = 255, g = 255, b = 255, a = 255;
        GameViewColorRGBA() = default;
        GameViewColorRGBA(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}
    };

    /// <summary>
    /// The "Game View" - a separate window showing the same simulation 
    /// as the wallpaper, but with interactive controls. Spawned from 
    /// system tray when user wants to interact.
    /// </summary>
    class GameViewWindow {
    public:
        struct Config {
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            
            int windowWidth = 1200;
            int windowHeight = 750;
            std::string windowTitle = "Polygonal Primordials - Game View";
            
            GameViewColorRGBA topBarColor = {15, 18, 25, 255};
            GameViewColorRGBA bottomBarColor = {15, 18, 25, 255};
            GameViewColorRGBA accentColor = {100, 180, 255, 255};
            GameViewColorRGBA textColor = {230, 235, 245, 255};
            GameViewColorRGBA warningColor = {255, 200, 100, 255};
            GameViewColorRGBA successColor = {130, 220, 150, 255};
            GameViewColorRGBA dangerColor = {255, 100, 100, 255};
            GameViewColorRGBA buttonColor = {45, 55, 70, 255};
            GameViewColorRGBA buttonHoverColor = {65, 85, 120, 255};
        };

        explicit GameViewWindow(const Config& config);
        ~GameViewWindow();

        void render();
        void update(float deltaTime);
        GameViewAction handleInput();

        bool isVisible() const { return m_visible; }
        void show();
        void hide();
        void toggle();
        
        void setPaused(bool paused);
        void setSimulationSpeed(float speed) { m_simSpeed = speed; }
        void setEntityCount(int count) { m_entityCount = count; }
        void setTickCount(uint64_t tick) { m_tickCount = tick; }
        void setFPS(float fps) { m_fps = fps; }
        void setThemeName(const std::string& name) { m_themeName = name; }

        using ActionCallback = std::function<void(GameViewAction)>;
        void setOnAction(ActionCallback cb) { m_actionCb = std::move(cb); }

    private:
        void renderTopBar();
        void renderBottomBar();
        void renderSimulationViewport();
        void renderSidePanel();
        void renderToolButton(int x, int y, int w, int h, const std::string& icon, 
                                const std::string& tooltip, bool active = false);
        void renderText(const std::string& text, int x, int y, int size, 
                         GameViewColorRGBA color, bool centered = false);

        Config m_config;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        bool m_visible = false;
        bool m_paused = false;
        float m_simSpeed = 1.0f;
        int m_entityCount = 0;
        uint64_t m_tickCount = 0;
        float m_fps = 60.0f;
        std::string m_themeName = "Deep Sea";
        
        bool m_showSidePanel = true;
        
        ActionCallback m_actionCb;
    };

} // namespace ShapeEngine::GameView
