#pragma once

#include <string>
#include <vector>
#include <functional>
#include <cstdint>
#include <memory>
#include <atomic>

namespace ShapeEngine::Wallpaper {

class SystemTray {
public:
    using MenuCallback = std::function<void(int itemId)>;
    using ThemeCallback = std::function<void(const std::string& themeId)>;
    using SpeedCallback = std::function<void(float speed)>;
    using ProfileCallback = std::function<void(const std::string& profileId)>;

    struct TrayConfig {
        std::string appName = "Polygonal Primordials";
        std::string tooltip = "Polygonal Primordials - Running";
        std::string iconPath = "assets/icons/tray.png";
        bool showNotifications = true;
    };

    enum MenuId : int {
        MENU_OPEN_GAME = 1001,
        MENU_PAUSE_TOGGLE = 1002,
        MENU_THEME_HEADER = 1010,
        MENU_THEME_BASE = 1100,    // 1100..1199 for themes
        MENU_GET_THEMES = 1200,
        MENU_SPEED_HEADER = 1210,
        MENU_SPEED_SLOW = 1220,
        MENU_SPEED_NORMAL = 1221,
        MENU_SPEED_FAST = 1222,
        MENU_PROFILE_HEADER = 1230,
        MENU_PROFILE_BASE = 1300,  // 1300..1399 for profiles
        MENU_PROFILE_SAVE = 1400,
        MENU_SETTINGS = 1500,
        MENU_DIARY = 1501,
        MENU_SCREENSHOT = 1502,
        MENU_ABOUT = 1600,
        MENU_QUIT = 1700,
    };

    SystemTray();
    ~SystemTray();

    bool initialize(const TrayConfig& config);
    void shutdown();

    void setIcon(const std::string& iconPath);
    void setTooltip(const std::string& tooltip);

    void showNotification(const std::string& title, 
                           const std::string& body,
                           int timeoutMs = 5000);

    void setThemes(const std::vector<std::pair<std::string, std::string>>& themes,
                   const std::string& currentThemeId);

    void setProfiles(const std::vector<std::string>& profiles,
                      const std::string& currentProfileId);

    void setPaused(bool paused);

    void setMenuCallback(MenuCallback cb) { m_menuCb = std::move(cb); }
    void setThemeCallback(ThemeCallback cb) { m_themeCb = std::move(cb); }
    void setSpeedCallback(SpeedCallback cb) { m_speedCb = std::move(cb); }
    void setProfileCallback(ProfileCallback cb) { m_profileCb = std::move(cb); }

    void poll();

    bool shouldQuit() const { return m_shouldQuit; }

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    MenuCallback m_menuCb;
    ThemeCallback m_themeCb;
    SpeedCallback m_speedCb;
    ProfileCallback m_profileCb;

    std::atomic<bool> m_shouldQuit{ false };
    bool m_paused = false;
};

} // namespace ShapeEngine::Wallpaper
