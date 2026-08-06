// Engine/UI/Widgets/NotificationToast.h
#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <deque>
#include <functional>

namespace ShapeEngine::UI {

    enum class ToastType {
        Info,
        Success,
        Warning,
        Error,
        Achievement,
        AchievementHidden
    };

    struct ToastColorRGBA {
        Uint8 r = 255, g = 255, b = 255, a = 255;
        ToastColorRGBA() = default;
        ToastColorRGBA(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}
    };

    /// <summary>
    /// Toast notifications that appear top-right and auto-dismiss.
    /// Used for: achievements, errors, info messages, mod events.
    /// </summary>
    class NotificationToast {
    public:
        struct Toast {
            std::string title;
            std::string message;
            ToastType type = ToastType::Info;
            float timeRemaining = 0.0f;
            float duration = 4.0f;
            ToastColorRGBA color;
            std::string iconPath;
            uint64_t timestamp = 0;
        };

        struct Config {
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            
            float defaultDuration = 4.0f;
            float stackOffset = 10.0f;
            float slideInDuration = 0.3f;
            float fadeOutDuration = 0.5f;
            int maxVisible = 5;
            
            int toastWidth = 320;
            int toastHeight = 70;
            int margin = 20;
            
            ToastColorRGBA infoColor = {70, 130, 180, 255};
            ToastColorRGBA successColor = {80, 170, 100, 255};
            ToastColorRGBA warningColor = {220, 160, 60, 255};
            ToastColorRGBA errorColor = {200, 70, 70, 255};
            ToastColorRGBA achievementColor = {200, 150, 255, 255};
            ToastColorRGBA achievementHiddenColor = {120, 80, 180, 255};
        };

        explicit NotificationToast(const Config& config);
        ~NotificationToast();

        void render();
        void update(float deltaTime);

        void show(const std::string& title, const std::string& message, 
                   ToastType type = ToastType::Info, float duration = 0.0f);
        
        void info(const std::string& title, const std::string& message) {
            show(title, message, ToastType::Info);
        }
        void success(const std::string& title, const std::string& message) {
            show(title, message, ToastType::Success);
        }
        void warning(const std::string& title, const std::string& message) {
            show(title, message, ToastType::Warning, 6.0f);
        }
        void error(const std::string& title, const std::string& message) {
            show(title, message, ToastType::Error, 8.0f);
        }
        void achievement(const std::string& title, const std::string& message, 
                          bool hidden = false) {
            show(title, message, hidden ? ToastType::AchievementHidden : 
                                            ToastType::Achievement, 6.0f);
        }

        using ToastClickedCallback = std::function<void(const Toast&)>;
        void setOnToastClicked(ToastClickedCallback cb) { m_onClick = std::move(cb); }

        void clear();
        int getActiveCount() const { return (int)m_toasts.size(); }

    private:
        void renderToast(int index, const Toast& toast, float slideProgress);
        void renderIcon(SDL_Rect iconRect, ToastType type);
        void renderText(const std::string& text, int x, int y, int size, 
                         ToastColorRGBA color, bool centered = false);
        void drawCircle(int cx, int cy, int radius, int segments);
        ToastColorRGBA getColorForType(ToastType type);

        Config m_config;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        std::deque<Toast> m_toasts;
        ToastClickedCallback m_onClick;
    };

} // namespace ShapeEngine::UI
