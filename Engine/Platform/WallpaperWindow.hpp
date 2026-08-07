#pragma once

#include "Core/Platform.hpp"
#include <SDL3/SDL.h>
#include <string>

namespace Shape::Platform {

enum class WallpaperMode {
    BehindDesktop, // Windows WorkerW hook (behind desktop icons)
    Fullscreen,    // Always-on-bottom fallback
    Windowed       // Debug / regular windowed mode
};

class WallpaperWindow {
public:
    WallpaperWindow() = default;
    ~WallpaperWindow();

    bool Initialize(const std::string& title, i32 width, i32 height, WallpaperMode mode = WallpaperMode::BehindDesktop);
    void Shutdown();

    void SetClickThrough(bool enabled);
    void BringToBottom();
    bool IsObscured() const;

    SDL_Window* GetSDLWindow() const { return m_Window; }
    WallpaperMode GetMode() const { return m_Mode; }

    i32 GetWidth() const { return m_Width; }
    i32 GetHeight() const { return m_Height; }

private:
    bool SetupWorkerWHook();

    SDL_Window* m_Window = nullptr;
    WallpaperMode m_Mode = WallpaperMode::BehindDesktop;
    i32 m_Width = 1920;
    i32 m_Height = 1080;
    bool m_IsClickThrough = true;
};

} // namespace Shape::Platform
