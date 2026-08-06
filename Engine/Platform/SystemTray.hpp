#pragma once

#include "Core/Platform.hpp"
#include <string>
#include <vector>
#include <functional>

struct SDL_Tray;
struct SDL_TrayMenu;
struct SDL_TrayEntry;

namespace Shape::Platform {

enum class TrayMenuItem {
    OpenGameView,
    TogglePause,
    SpeedSlow,
    SpeedNormal,
    SpeedFast,
    ThemeSelect,
    OpenSettings,
    Quit
};

class SystemTray {
public:
    using ActionCallback = std::function<void(TrayMenuItem, const std::string& extraData)>;

    SystemTray() = default;
    ~SystemTray();

    bool Initialize(const std::string& appName, const std::string& iconPath);
    void Shutdown();

    void SetActionCallback(ActionCallback callback) { m_Callback = std::move(callback); }
    
    void UpdateThemeList(const std::vector<std::string>& themeNames, const std::string& activeTheme);
    void SetPaused(bool paused);
    void SetTooltip(const std::string& tooltip);

private:
    SDL_Tray* m_Tray = nullptr;
    SDL_TrayMenu* m_Menu = nullptr;
    SDL_TrayEntry* m_PauseEntry = nullptr;
    SDL_TrayMenu* m_ThemeSubmenu = nullptr;
    ActionCallback m_Callback;
    bool m_IsInitialized = false;
};

} // namespace Shape::Platform
