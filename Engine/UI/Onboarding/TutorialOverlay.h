// Engine/UI/Onboarding/TutorialOverlay.h
#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <functional>

namespace ShapeEngine::UI {

    struct TutorialColorRGBA {
        Uint8 r = 255, g = 255, b = 255, a = 255;
        TutorialColorRGBA() = default;
        TutorialColorRGBA(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}
    };

    /// <summary>
    /// Interactive tutorial system for first-time players.
    /// Shows step-by-step guidance with text and visual hints.
    /// </summary>
    class TutorialOverlay {
    public:
        struct Step {
            std::string id;
            std::string title;
            std::string body;
            float highlightX = -1, highlightY = -1;
            float highlightW = -1, highlightH = -1;
            float duration = 0;  // 0 = manual advance
            std::function<bool()> autoComplete;  // Optional condition
        };

        struct Config {
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            
            TutorialColorRGBA backgroundColor = {0, 0, 0, 180};
            TutorialColorRGBA cardColor = {30, 35, 50, 250};
            TutorialColorRGBA borderColor = {100, 180, 255, 255};
            TutorialColorRGBA textColor = {255, 255, 255, 255};
            TutorialColorRGBA secondaryTextColor = {200, 210, 230, 255};
            TutorialColorRGBA accentColor = {100, 180, 255, 255};
            TutorialColorRGBA buttonColor = {50, 90, 140, 255};
            TutorialColorRGBA buttonHoverColor = {70, 120, 180, 255};
            
            int cardWidth = 500;
            int cardHeight = 250;
            
            std::vector<Step> steps = {
                {"welcome", "Welcome to Polygonal Primordials!",
                 "You're about to watch digital life evolve. Creatures with "
                 "heritable traits will hunt, reproduce, and adapt over time.\n\n"
                 "Let's take a quick tour!", 0, 0, 0, 0, 6.0f, nullptr},
                
                {"camera", "Controlling the Camera",
                 "Use WASD or Arrow Keys to pan the camera.\n"
                 "Scroll the mouse wheel to zoom in/out.", 0, 0, 0, 0, 8.0f, nullptr},
                
                {"select", "Selecting Entities",
                 "Click on any creature to select it and see its genome.",
                 0, 0, 0, 0, 6.0f, nullptr},
                
                {"inspector", "The Inspector",
                 "The Inspector panel shows you the traits and lineage of "
                 "selected creatures. Press TAB to toggle it.", 0, 0, 0, 0, 6.0f, nullptr},
                
                {"speed", "Simulation Speed",
                 "Use [ and ] keys to slow down or speed up time.\n"
                 "Press SPACE to pause.", 0, 0, 0, 0, 5.0f, nullptr},
                
                {"menu", "Settings & Menus",
                 "Press ESC anytime to open the pause menu.\n"
                 "Access Settings to customize your experience.",
                 0, 0, 0, 0, 5.0f, nullptr},
                
                {"done", "You're Ready!",
                 "That's the basics! Watch your ecosystem evolve and "
                 "discover emergent behaviors. Have fun!\n\n"
                 "Press ESC to open the help system anytime.", 0, 0, 0, 0, 8.0f, nullptr}
            };
        };

        explicit TutorialOverlay(const Config& config);
        ~TutorialOverlay();

        void render();
        void update(float deltaTime);
        bool handleInput();

        void start();
        void stop();
        void skip();

        bool isActive() const { return m_active; }
        bool isComplete() const { return m_complete; }
        bool isSkipped() const { return m_skipped; }
        int getCurrentStepIndex() const { return m_currentStep; }
        void nextStep();
        void previousStep();

        // ─── Callbacks ───────────────────────────────────────
        using CompleteCallback = std::function<void()>;
        void setOnComplete(CompleteCallback cb) { m_onComplete = std::move(cb); }
        using SkipCallback = std::function<void()>;
        void setOnSkipped(SkipCallback cb) { m_onSkipped = std::move(cb); }

    private:
        void renderCard();
        void renderHighlight();
        void renderNavigation();
        void renderText(const std::string& text, int x, int y, int size, 
                         TutorialColorRGBA color, bool centered = false);
        void renderWrappedText(const std::string& text, int x, int y, int w, 
                                int size, TutorialColorRGBA color);
        bool button(int x, int y, int w, int h, const std::string& label, 
                     TutorialColorRGBA color);

        Config m_config;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        bool m_active = false;
        bool m_complete = false;
        bool m_skipped = false;
        int m_currentStep = 0;
        float m_stepTimer = 0.0f;
        
        float m_cardOffset = 100.0f;
        float m_cardTarget = 0.0f;
        
        CompleteCallback m_onComplete;
        SkipCallback m_onSkipped;
    };

} // namespace ShapeEngine::UI
