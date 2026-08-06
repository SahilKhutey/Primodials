#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <algorithm>

struct SDL_Window;

namespace ShapeEngine::Wallpaper {

    class WallpaperWindow {
    public:
        enum class Mode {
            BehindDesktop,
            FullscreenAtBottom,
            WindowedNormal,
        };

        enum class RenderStatus {
            Visible,
            ObscuredByFullscreen,
            Minimized,
            Hidden,
        };

        WallpaperWindow();
        ~WallpaperWindow();

        bool initialize(Mode mode, uint32_t width, uint32_t height);

        SDL_Window* getSDLWindow() const { return m_window; }
        void* getGLContext() const;
        void present();
        RenderStatus getStatus() const;

        void setClickThrough(bool enable);
        void setOpacity(float opacity);
        void bringToBottom();
        void resize(uint32_t width, uint32_t height);

        void shutdown();

    private:
        Mode m_mode = Mode::FullscreenAtBottom;
        SDL_Window* m_window = nullptr;
        void* m_glContext = nullptr;
        uint32_t m_width = 0, m_height = 0;
        bool m_clickThrough = true;
        float m_opacity = 1.0f;

        struct PlatformData;
        std::unique_ptr<PlatformData> m_platform;

        bool initializeBehindDesktop_Windows(uint32_t w, uint32_t h);
        bool initializeFullscreenAtBottom(uint32_t w, uint32_t h);
        bool initializeWindowedNormal(uint32_t w, uint32_t h);
    };

} // namespace ShapeEngine::Wallpaper
