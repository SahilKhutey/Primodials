// Engine/UI/Loading/LoadingScreen.h
#pragma once

#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <atomic>
#include <chrono>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace ShapeEngine::UI {

    struct ColorRGBA {
        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
        float a = 1.0f;

        ColorRGBA() = default;
        ColorRGBA(float r_, float g_, float b_, float a_ = 1.0f) : r(r_), g(g_), b(b_), a(a_) {}
    };

    enum class LoadingState {
        Initializing,           // Engine startup
        LoadingAssets,           // Textures, audio
        LoadingThemes,           // Theme files
        GeneratingWorld,         // World generation
        ConnectingMultiplayer,   // Network setup
        Saving,                  // Save operation
        Loading,                 // Load operation
        GeneratingPreview,       // Workshop preview
        Complete
    };

    /// <summary>
    /// Manages loading screens with progress bar, status text, and tips.
    /// Renders on top of everything during async operations.
    /// </summary>
    class LoadingScreen {
    public:
        struct Config {
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            std::string titleScreenImage = "assets/ui/splash.png";
            std::string loadingBarImage = "assets/ui/loading_bar.png";
            std::string loadingBarFillImage = "assets/ui/loading_bar_fill.png";
            std::string logoImage = "assets/ui/logo.png";
            std::vector<std::string> tips = {
                "Creatures evolve over time - watch their traits change!",
                "Predators emerge naturally from evolutionary pressure.",
                "Try increasing mutation rate for more diversity.",
                "Press F5 to save interesting moments.",
                "The simulation is fully deterministic - same seed = same result.",
                "Use the heatmap to visualize population density.",
                "Slower simulation speed = more time to observe changes."
            };
            
            // Visual style
            ColorRGBA backgroundColor = ColorRGBA(0.02f, 0.02f, 0.05f, 1.0f);
            ColorRGBA textColor = ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f);
            ColorRGBA accentColor = ColorRGBA(0.4f, 0.9f, 1.0f, 1.0f);
            ColorRGBA progressBarColor = ColorRGBA(0.3f, 0.7f, 1.0f, 1.0f);
            
            // Animation
            float fadeInDuration = 0.5f;
            float fadeOutDuration = 0.3f;
            float rotationSpeed = 45.0f;  // degrees per second
            
            bool showTips = true;
            bool showProgressBar = true;
            bool showLogo = true;
            bool animateBackground = true;
        };

        explicit LoadingScreen(const Config& config);
        ~LoadingScreen();

        // ─── State Management ─────────────────────────────────
        void setState(LoadingState state, const std::string& statusText = "");
        void setProgress(float progress);  // 0.0 - 1.0
        void setStatusText(const std::string& text);
        void setTip(const std::string& tip);
        void setError(const std::string& error);
        
        void startLoading(const std::string& initialStatus = "Loading...");
        void finishLoading();
        bool isLoading() const { return m_isLoading; }

        // ─── Async Operations ─────────────────────────────────
        template<typename Func>
        void runAsync(Func&& task, const std::string& statusText = "") {
            setState(LoadingState::LoadingAssets, statusText);
            
            std::thread([this, task = std::forward<Func>(task)]() {
                try {
                    task();
                    finishLoading();
                } catch (...) {
                    setError("Loading failed");
                    finishLoading();
                }
            }).detach();
        }

        // ─── Rendering ───────────────────────────────────────
        void render();
        void update(float deltaTime);

        void loadAssetsAsync(const std::vector<std::string>& assetPaths);

    private:
        void renderBackground();
        void renderLogo();
        void renderProgressBar();
        void renderStatusText();
        void renderTip();
        void renderError();
        void renderAnimatedElements(float deltaTime);
        
        void cycleTip();
        void startFadeIn();
        void startFadeOut();
        
        SDL_Texture* loadTexture(const std::string& path);
        void renderText(const std::string& text, float x, float y, float size, 
                         const ColorRGBA& color);

        Config m_config;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        
        // State
        LoadingState m_currentState = LoadingState::Initializing;
        std::atomic<float> m_progress{0.0f};
        std::string m_statusText;
        std::string m_currentTip;
        std::string m_errorMessage;
        std::atomic<bool> m_isLoading{false};
        std::atomic<bool> m_hasError{false};
        
        // Animation
        float m_fadeAlpha = 0.0f;
        float m_rotationAngle = 0.0f;
        float m_timeSinceLastTip = 0.0f;
        int m_currentTipIndex = 0;
        bool m_fadingIn = true;
        bool m_fadingOut = false;
        
        // Textures (cached)
        SDL_Texture* m_titleTexture = nullptr;
        SDL_Texture* m_logoTexture = nullptr;
        SDL_Texture* m_progressBarTexture = nullptr;
        SDL_Texture* m_progressBarFillTexture = nullptr;
        
        std::chrono::steady_clock::time_point m_startTime;
    };

} // namespace ShapeEngine::UI
