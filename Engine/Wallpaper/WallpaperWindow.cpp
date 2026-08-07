#include "WallpaperWindow.h"
#include "Core/Logger.hpp"

#include <SDL3/SDL.h>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #include <dwmapi.h>
#ifdef _MSC_VER
    #pragma comment(lib, "dwmapi.lib")
#endif
#endif

namespace ShapeEngine::Wallpaper {

    struct WallpaperWindow::PlatformData {
#ifdef _WIN32
        HWND progman = nullptr;
        HWND workerW = nullptr;
        HWND ourHwnd = nullptr;
#endif
    };

    WallpaperWindow::WallpaperWindow() : m_platform(std::make_unique<PlatformData>()) {}
    WallpaperWindow::~WallpaperWindow() { shutdown(); }

    bool WallpaperWindow::initialize(Mode mode, uint32_t width, uint32_t height) {
        m_mode = mode;
        m_width = width;
        m_height = height;

        switch (mode) {
            case Mode::BehindDesktop:
#ifdef _WIN32
                return initializeBehindDesktop_Windows(width, height);
#else
                SHAPE_LOG_WARN("BehindDesktop mode is Windows-only; falling back to FullscreenAtBottom");
                return initializeFullscreenAtBottom(width, height);
#endif

            case Mode::FullscreenAtBottom:
                return initializeFullscreenAtBottom(width, height);

            case Mode::WindowedNormal:
                return initializeWindowedNormal(width, height);
        }
        return false;
    }

#ifdef _WIN32
    bool WallpaperWindow::initializeBehindDesktop_Windows(uint32_t w, uint32_t h) {
        SHAPE_LOG_INFO("Initializing wallpaper mode: behind desktop icons (Windows WorkerW)");

        m_platform->progman = FindWindowW(L"Progman", nullptr);
        if (!m_platform->progman) {
            SHAPE_LOG_ERROR("Could not find Progman window");
            return false;
        }

        SendMessageTimeoutW(m_platform->progman, 0x052C, 0, 0, 
                              SMTO_NORMAL, 1000, nullptr);

        EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
            HWND defView = FindWindowExW(hwnd, nullptr, L"SHELLDLL_DefView", nullptr);
            if (defView != nullptr) {
                HWND nextWorker = FindWindowExW(nullptr, hwnd, L"WorkerW", nullptr);
                HWND* pWorker = reinterpret_cast<HWND*>(lParam);
                *pWorker = nextWorker;
                return FALSE;
            }
            return TRUE;
        }, reinterpret_cast<LPARAM>(&m_platform->workerW));

        if (!m_platform->workerW) {
            SHAPE_LOG_ERROR("Could not find WorkerW");
            return false;
        }

        SDL_WindowFlags flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL;
        m_window = SDL_CreateWindow("Primordials Wallpaper", 
                                      (int)w, (int)h, flags);
        if (!m_window) {
            SHAPE_LOG_ERROR("Failed to create SDL window: {}", SDL_GetError());
            return false;
        }

        HWND ourHwnd = static_cast<HWND>(SDL_GetPointerProperty(
            SDL_GetWindowProperties(m_window), 
            SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));

        if (!ourHwnd) {
            SHAPE_LOG_ERROR("Could not get native HWND");
            return false;
        }

        SetParent(ourHwnd, m_platform->workerW);
        SetWindowLongPtrW(ourHwnd, GWL_STYLE, WS_CHILD | WS_VISIBLE);

        RECT rc;
        GetClientRect(m_platform->workerW, &rc);
        SetWindowPos(ourHwnd, nullptr, 0, 0, 
                     rc.right - rc.left, rc.bottom - rc.top,
                     SWP_NOZORDER | SWP_NOACTIVATE);

        m_platform->ourHwnd = ourHwnd;

        SHAPE_LOG_INFO("Wallpaper embedded in WorkerW");
        return true;
    }
#endif

    bool WallpaperWindow::initializeFullscreenAtBottom(uint32_t w, uint32_t h) {
        SHAPE_LOG_INFO("Initializing wallpaper mode: fullscreen at bottom");

        SDL_WindowFlags flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL;
        m_window = SDL_CreateWindow("Polygonal Primordials Wallpaper", 
                                      (int)w, (int)h, flags);
        if (!m_window) {
            SHAPE_LOG_ERROR("Failed to create window: {}", SDL_GetError());
            return false;
        }

#ifdef _WIN32
        HWND hwnd = static_cast<HWND>(SDL_GetPointerProperty(
            SDL_GetWindowProperties(m_window), 
            SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));
        if (hwnd) {
            m_platform->ourHwnd = hwnd;
            SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, (int)w, (int)h, SWP_NOACTIVATE);
            LONG exStyle = GetWindowLongW(hwnd, GWL_EXSTYLE);
            SetWindowLongW(hwnd, GWL_EXSTYLE, exStyle | WS_EX_TRANSPARENT | WS_EX_LAYERED);
        }
#endif
        return true;
    }

    bool WallpaperWindow::initializeWindowedNormal(uint32_t w, uint32_t h) {
        SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
        m_window = SDL_CreateWindow("Polygonal Primordials (Debug)", 
                                      (int)w, (int)h, flags);
        return m_window != nullptr;
    }

    void* WallpaperWindow::getGLContext() const {
        if (!m_window) return nullptr;
        return SDL_GL_GetCurrentContext();
    }

    void WallpaperWindow::present() {
        if (m_window) {
            SDL_GL_SwapWindow(m_window);
        }
    }

    WallpaperWindow::RenderStatus WallpaperWindow::getStatus() const {
        if (!m_window) return RenderStatus::Hidden;

        Uint32 flags = SDL_GetWindowFlags(m_window);
        if (flags & SDL_WINDOW_MINIMIZED) return RenderStatus::Minimized;
        if (flags & SDL_WINDOW_HIDDEN)   return RenderStatus::Hidden;

#ifdef _WIN32
        if (m_platform->ourHwnd) {
            HWND foreground = GetForegroundWindow();
            if (foreground && foreground != m_platform->ourHwnd && foreground != m_platform->workerW) {
                RECT rc;
                GetWindowRect(foreground, &rc);
                RECT desk;
                GetWindowRect(GetDesktopWindow(), &desk);
                if (rc.left <= desk.left && rc.top <= desk.top && 
                    rc.right >= desk.right && rc.bottom >= desk.bottom) {
                    return RenderStatus::ObscuredByFullscreen;
                }
            }
        }
#endif
        return RenderStatus::Visible;
    }

    void WallpaperWindow::setClickThrough(bool enable) {
        m_clickThrough = enable;
#ifdef _WIN32
        if (m_platform->ourHwnd) {
            LONG exStyle = GetWindowLongW(m_platform->ourHwnd, GWL_EXSTYLE);
            if (enable) exStyle |= WS_EX_TRANSPARENT;
            else        exStyle &= ~WS_EX_TRANSPARENT;
            SetWindowLongW(m_platform->ourHwnd, GWL_EXSTYLE, exStyle);
        }
#endif
    }

    void WallpaperWindow::setOpacity(float opacity) {
        m_opacity = std::clamp(opacity, 0.0f, 1.0f);
    }

    void WallpaperWindow::bringToBottom() {
#ifdef _WIN32
        if (m_platform->ourHwnd) {
            SetWindowPos(m_platform->ourHwnd, HWND_BOTTOM, 0, 0, 0, 0,
                         SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        }
#endif
    }

    void WallpaperWindow::resize(uint32_t w, uint32_t h) {
        m_width = w;
        m_height = h;
        if (m_window) SDL_SetWindowSize(m_window, (int)w, (int)h);
    }

    void WallpaperWindow::shutdown() {
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
        m_glContext = nullptr;
    }

} // namespace ShapeEngine::Wallpaper
