#include "SystemTray.h"
#include "Core/Logger.hpp"

#include <SDL3/SDL.h>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #include <shellapi.h>
    #include <strsafe.h>
#ifdef _MSC_VER
    #pragma comment(lib, "shell32.lib")
    #pragma comment(lib, "user32.lib")
    #pragma comment(lib, "gdi32.lib")
#endif
#endif

namespace ShapeEngine::Wallpaper {

    struct SystemTray::Impl {
        TrayConfig config;

#ifdef _WIN32
        HWND hwnd = nullptr;
        NOTIFYICONDATAW nid{};
        HMENU hMenu = nullptr;
        HICON hIcon = nullptr;

        void createWindow() {
            WNDCLASSW wc = {};
            wc.lpfnWndProc = DefWindowProcW;
            wc.hInstance = GetModuleHandle(nullptr);
            wc.lpszClassName = L"PPTrayClass";
            RegisterClassW(&wc);

            hwnd = CreateWindowExW(0, L"PPTrayClass", L"PP", 0, 0, 0, 0, 0,
                                    HWND_MESSAGE, nullptr, wc.hInstance, nullptr);

            SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)TrayWndProc);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
        }

        static LRESULT CALLBACK TrayWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            if (msg == WM_USER + 1) {
                if (lParam == WM_RBUTTONUP || lParam == WM_CONTEXTMENU) {
                    Impl* self = reinterpret_cast<Impl*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
                    if (self) self->showContextMenu();
                } else if (lParam == WM_LBUTTONDBLCLK) {
                    if (s_menuCb) s_menuCb(MENU_OPEN_GAME);
                }
                return 0;
            } else if (msg == WM_COMMAND) {
                int wmId = LOWORD(wParam);
                if (s_menuCb) s_menuCb(wmId);
                return 0;
            }
            return DefWindowProcW(hWnd, msg, wParam, lParam);
        }

        void showContextMenu() {
            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hwnd);
            TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_VERPOSANIMATION,
                          pt.x, pt.y, 0, hwnd, nullptr);
            PostMessage(hwnd, WM_NULL, 0, 0);
        }

        void addIcon() {
            ZeroMemory(&nid, sizeof(nid));
            nid.cbSize = sizeof(nid);
            nid.hWnd = hwnd;
            nid.uID = 1;
            nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            nid.uCallbackMessage = WM_USER + 1;
            nid.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
            StringCchCopyW(nid.szTip, 128, L"Polygonal Primordials Wallpaper");
            Shell_NotifyIconW(NIM_ADD, &nid);
        }

        void updateTooltip(const std::string& tip) {
            std::wstring wtip(tip.begin(), tip.end());
            StringCchCopyW(nid.szTip, 128, wtip.c_str());
            Shell_NotifyIconW(NIM_MODIFY, &nid);
        }
#endif

        static MenuCallback s_menuCb;
    };

    SystemTray::MenuCallback SystemTray::Impl::s_menuCb = nullptr;

    SystemTray::SystemTray() : m_impl(std::make_unique<Impl>()) {}
    SystemTray::~SystemTray() { shutdown(); }

    bool SystemTray::initialize(const TrayConfig& config) {
        m_impl->config = config;
        Impl::s_menuCb = m_menuCb;

#ifdef _WIN32
        m_impl->createWindow();
        m_impl->addIcon();
        SHAPE_LOG_INFO("System tray initialized (Windows native Shell_NotifyIcon)");
#else
        SHAPE_LOG_INFO("System tray initialized (Fallback mode)");
#endif
        return true;
    }

    void SystemTray::shutdown() {
#ifdef _WIN32
        Shell_NotifyIconW(NIM_DELETE, &m_impl->nid);
        if (m_impl->hwnd) DestroyWindow(m_impl->hwnd);
#endif
    }

    void SystemTray::setIcon(const std::string& iconPath) {
#ifdef _WIN32
        if (m_impl->hIcon) DestroyIcon(m_impl->hIcon);
        m_impl->hIcon = (HICON)LoadImageW(nullptr, std::wstring(iconPath.begin(), 
            iconPath.end()).c_str(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
        if (m_impl->hIcon) {
            m_impl->nid.hIcon = m_impl->hIcon;
            Shell_NotifyIconW(NIM_MODIFY, &m_impl->nid);
        }
#endif
    }

    void SystemTray::setTooltip(const std::string& tooltip) {
        m_impl->config.tooltip = tooltip;
#ifdef _WIN32
        m_impl->updateTooltip(tooltip);
#endif
    }

    void SystemTray::showNotification(const std::string& title, 
                                       const std::string& body,
                                       int timeoutMs) {
#ifdef _WIN32
        m_impl->nid.uFlags |= NIF_INFO;
        std::wstring wtitle(title.begin(), title.end());
        std::wstring wbody(body.begin(), body.end());
        StringCchCopyW(m_impl->nid.szInfoTitle, 64, wtitle.c_str());
        StringCchCopyW(m_impl->nid.szInfo, 256, wbody.c_str());
        m_impl->nid.dwInfoFlags = NIIF_INFO;
        m_impl->nid.uTimeout = timeoutMs;
        Shell_NotifyIconW(NIM_MODIFY, &m_impl->nid);
        m_impl->nid.uFlags &= ~NIF_INFO;
#endif
    }

    void SystemTray::setThemes(
        const std::vector<std::pair<std::string, std::string>>& themes,
        const std::string& currentThemeId) {
#ifdef _WIN32
        if (m_impl->hMenu) DestroyMenu(m_impl->hMenu);
        m_impl->hMenu = CreatePopupMenu();

        AppendMenuW(m_impl->hMenu, MF_STRING, MENU_OPEN_GAME, L"Open Game View");
        AppendMenuW(m_impl->hMenu, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(m_impl->hMenu, MF_STRING | (m_paused ? MF_CHECKED : MF_UNCHECKED), 
                    MENU_PAUSE_TOGGLE, m_paused ? L"Resume" : L"Pause");
        AppendMenuW(m_impl->hMenu, MF_SEPARATOR, 0, nullptr);

        HMENU themeMenu = CreatePopupMenu();
        for (size_t i = 0; i < themes.size(); ++i) {
            UINT flags = MF_STRING;
            if (themes[i].first == currentThemeId) flags |= MF_CHECKED;
            AppendMenuW(themeMenu, flags, MENU_THEME_BASE + (int)i,
                       std::wstring(themes[i].second.begin(), 
                                    themes[i].second.end()).c_str());
        }
        AppendMenuW(themeMenu, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(themeMenu, MF_STRING, MENU_GET_THEMES, L"Get More Themes...");
        AppendMenuW(m_impl->hMenu, MF_POPUP, (UINT_PTR)themeMenu, L"Theme");
        AppendMenuW(m_impl->hMenu, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(m_impl->hMenu, MF_STRING, MENU_QUIT, L"Quit Wallpaper");
#endif
    }

    void SystemTray::setProfiles(const std::vector<std::string>& profiles,
                                  const std::string& currentProfileId) {
        (void)profiles;
        (void)currentProfileId;
    }

    void SystemTray::setPaused(bool paused) {
        m_paused = paused;
    }

    void SystemTray::poll() {
#ifdef _WIN32
        MSG msg;
        while (PeekMessageW(&msg, m_impl->hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
#endif
    }

} // namespace ShapeEngine::Wallpaper
