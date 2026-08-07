// Engine/UI/Performance/PerformanceOverlay.h
#pragma once

#include <SDL3/SDL.h>
#include <deque>
#include <string>

namespace ShapeEngine::UI {

    struct PerformanceColorRGBA {
        Uint8 r = 255, g = 255, b = 255, a = 255;
        PerformanceColorRGBA() = default;
        PerformanceColorRGBA(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}
    };

    /// <summary>
    /// Real-time performance graph showing FPS, frame time, entity count.
    /// Toggle with F4. Useful for debugging and power users.
    /// </summary>
    class PerformanceOverlay {
    public:
        struct Config {
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            
            int overlayWidth = 300;
            int overlayHeight = 200;
            int margin = 20;
            int historySize = 120;  // Frames to keep
            
            PerformanceColorRGBA backgroundColor = {15, 18, 25, 220};
            PerformanceColorRGBA textColor = {230, 235, 245, 255};
            PerformanceColorRGBA fpsGoodColor = {130, 220, 150, 255};
            PerformanceColorRGBA fpsOkayColor = {255, 200, 100, 255};
            PerformanceColorRGBA fpsBadColor = {255, 100, 100, 255};
            PerformanceColorRGBA lineColor = {100, 180, 255, 255};
            PerformanceColorRGBA gridColor = {40, 45, 55, 255};
        };

        explicit PerformanceOverlay(const Config& config);
        ~PerformanceOverlay();

        void render();
        void update(float deltaTime);

        void show() { m_visible = true; }
        void hide() { m_visible = false; }
        void toggle() { m_visible = !m_visible; }
        bool isVisible() const { return m_visible; }

        // ─── Data input ───────────────────────────────────────
        void recordFrame(float deltaTime, int entityCount, float simSpeed);

        float getAvgFPS() const { return m_avgFPS; }
        float getAvgFrameTime() const { return m_avgFrameTime; }
        int getCurrentEntities() const { return m_currentEntities; }

    private:
        void renderFPSGraph();
        void renderStats();
        void renderText(const std::string& text, int x, int y, int size, 
                         PerformanceColorRGBA color);

        struct FrameData {
            float frameTime;  // milliseconds
            int entityCount;
        };

        Config m_config;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        bool m_visible = false;
        
        std::deque<FrameData> m_history;
        
        float m_avgFPS = 60.0f;
        float m_minFPS = 60.0f;
        float m_maxFPS = 60.0f;
        float m_avgFrameTime = 16.67f;
        int m_currentEntities = 0;
        float m_simSpeed = 1.0f;
    };

} // namespace ShapeEngine::UI
