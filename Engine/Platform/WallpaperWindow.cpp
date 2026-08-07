#include "Platform/WallpaperWindow.hpp"
#include "Core/Logger.hpp"
#include <SDL3/SDL.h>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace Shape::Platform {

WallpaperWindow::~WallpaperWindow() {
    Shutdown();
}

bool WallpaperWindow::Initialize(const std::string& title, i32 width, i32 height, WallpaperMode mode) {
    m_Width = width;
    m_Height = height;
    m_Mode = mode;

    Uint32 flags = SDL_WINDOW_OPENGL;

    if (m_Mode == WallpaperMode::Fullscreen || m_Mode == WallpaperMode::BehindDesktop) {
        flags |= SDL_WINDOW_BORDERLESS;
    }

    m_Window = SDL_CreateWindow(title.c_str(), width, height, flags);
    if (!m_Window) {
        SHAPE_LOG_ERROR("Failed to create SDL window for wallpaper: {}", SDL_GetError());
        return false;
    }

    if (m_Mode == WallpaperMode::BehindDesktop) {
#ifdef _WIN32
        if (!SetupWorkerWHook()) {
            SHAPE_LOG_WARN("WorkerW hook failed; falling back to Always-On-Bottom Fullscreen.");
            m_Mode = WallpaperMode::Fullscreen;
            SDL_SetWindowPosition(m_Window, 0, 0);
            SDL_SetWindowSize(m_Window, width, height);
            BringToBottom();
        }
#else
        SHAPE_LOG_INFO("Non-Windows OS detected; using Always-On-Bottom Fullscreen wallpaper mode.");
        m_Mode = WallpaperMode::Fullscreen;
        BringToBottom();
#endif
    } else if (m_Mode == WallpaperMode::Fullscreen) {
        SDL_SetWindowPosition(m_Window, 0, 0);
        SDL_SetWindowSize(m_Window, width, height);
        BringToBottom();
    }

    SetClickThrough(m_IsClickThrough);
    SHAPE_LOG_INFO("WallpaperWindow initialized successfully in mode {}", static_cast<int>(m_Mode));
    return true;
}

void WallpaperWindow::Shutdown() {
    if (m_Window) {
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
    }
}

bool WallpaperWindow::SetupWorkerWHook() {
#ifdef _WIN32
    HWND progman = FindWindowW(L"Progman", NULL);
    if (!progman) {
        SHAPE_LOG_WARN("Could not find Progman window.");
        return false;
    }

    // Send 0x052C to Progman to spawn a WorkerW window behind icons
    PDWORD_PTR result = nullptr;
    SendMessageTimeoutW(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, result);

    HWND workerW = nullptr;
    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        HWND shellView = FindWindowExW(hwnd, NULL, L"SHELLDLL_DefView", NULL);
        if (shellView != NULL) {
            HWND* pWorkerW = reinterpret_cast<HWND*>(lParam);
            *pWorkerW = FindWindowExW(NULL, hwnd, L"WorkerW", NULL);
            return FALSE;
        }
        return TRUE;
    }, reinterpret_cast<LPARAM>(&workerW));

    if (!workerW) {
        SHAPE_LOG_WARN("Could not locate WorkerW window handle.");
        return false;
    }

    SDL_PropertiesID props = SDL_GetWindowProperties(m_Window);
    HWND hwndSDL = static_cast<HWND>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL));
    if (!hwndSDL) {
        SHAPE_LOG_WARN("Could not obtain Win32 HWND from SDL_Window.");
        return false;
    }

    SetParent(hwndSDL, workerW);
    SHAPE_LOG_INFO("Successfully reparented SDL window to Windows WorkerW (Behind Desktop Icons).");
    return true;
#else
    return false;
#endif
}

void WallpaperWindow::SetClickThrough(bool enabled) {
    m_IsClickThrough = enabled;
#ifdef _WIN32
    if (m_Window) {
        SDL_PropertiesID props = SDL_GetWindowProperties(m_Window);
        HWND hwndSDL = static_cast<HWND>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL));
        if (hwndSDL) {
            LONG exStyle = GetWindowLongW(hwndSDL, GWL_EXSTYLE);
            if (m_IsClickThrough) {
                SetWindowLongW(hwndSDL, GWL_EXSTYLE, exStyle | WS_EX_TRANSPARENT | WS_EX_LAYERED);
            } else {
                SetWindowLongW(hwndSDL, GWL_EXSTYLE, exStyle & ~(WS_EX_TRANSPARENT | WS_EX_LAYERED));
            }
        }
    }
#endif
}

void WallpaperWindow::BringToBottom() {
#ifdef _WIN32
    if (m_Window) {
        SDL_PropertiesID props = SDL_GetWindowProperties(m_Window);
        HWND hwndSDL = static_cast<HWND>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL));
        if (hwndSDL) {
            SetWindowPos(hwndSDL, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        }
    }
#endif
}

bool WallpaperWindow::IsObscured() const {
#ifdef _WIN32
    HWND foreground = GetForegroundWindow();
    if (!foreground) return false;

    RECT fgRect, deskRect;
    GetWindowRect(foreground, &fgRect);
    GetWindowRect(GetDesktopWindow(), &deskRect);

    bool isFullscreen = (fgRect.left <= deskRect.left &&
                         fgRect.top <= deskRect.top &&
                         fgRect.right >= deskRect.right &&
                         fgRect.bottom >= deskRect.bottom);
    return isFullscreen;
#else
    return false;
#endif
}

} // namespace Shape::Platform
