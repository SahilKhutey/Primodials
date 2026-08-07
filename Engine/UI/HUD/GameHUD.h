// Engine/UI/HUD/GameHUD.h
#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <cstdint>

namespace ShapeEngine::UI {

    /// <summary>
    /// In-game HUD overlay shown during gameplay.
    /// Includes speed controls, pause button, quick stats.
    /// </summary>
    class GameHUD {
    public:
        GameHUD(SDL_Window* window, SDL_Renderer* renderer);
        ~GameHUD();

        void render();
        void update(float deltaTime);

        // Controls
        void setSimulationSpeed(float speed) { m_simSpeed = speed; }
        void setEntityCount(int count) { m_entityCount = count; }
        void setCurrentTick(uint64_t tick) { m_currentTick = tick; }
        void setPaused(bool paused) { m_paused = paused; }
        void setFPS(float fps) { m_fps = fps; }

        enum class HUDAction {
            None,
            PauseToggle,
            SpeedUp,
            SpeedDown,
            Settings,
            OpenInspector,
            ShowStats
        };

        HUDAction handleInput();
        bool isVisible() const { return m_visible; }

        void addNotification(const std::string& text, float duration = 3.0f, 
                            SDL_Color color = {255, 255, 255, 255});

    private:
        void renderTopBar();
        void renderBottomBar();
        void renderNotifications();
        void renderText(const std::string& text, int x, int y, int size, 
                         SDL_Color color);

        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        bool m_visible = true;
        
        // State
        float m_simSpeed = 1.0f;
        int m_entityCount = 0;
        uint64_t m_currentTick = 0;
        bool m_paused = false;
        float m_fps = 60.0f;
        bool m_showStats = false;
        
        // Notifications
        struct Notification {
            std::string text;
            float timeRemaining;
            SDL_Color color;
        };
        std::vector<Notification> m_notifications;
        
        // Styling
        SDL_Color m_bgColor = {0, 0, 0, 150};
        SDL_Color m_textColor = {255, 255, 255, 255};
        SDL_Color m_accentColor = {100, 200, 255, 255};
        SDL_Color m_buttonColor = {40, 50, 70, 200};
        SDL_Color m_warningColor = {255, 200, 100, 255};
    };

} // namespace ShapeEngine::UI
