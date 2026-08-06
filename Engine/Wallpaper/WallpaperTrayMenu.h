// Engine/Wallpaper/WallpaperTrayMenu.h
#pragma once

#include <string>
#include <vector>
#include <functional>

namespace ShapeEngine::Wallpaper {

    /// <summary>
    /// Builds the system tray context menu for wallpaper mode.
    /// Called by the actual system tray implementation.
    /// </summary>
    class WallpaperTrayMenu {
    public:
        struct MenuItem {
            std::string label;
            std::string id;
            bool isSeparator = false;
            bool isCheckable = false;
            bool isChecked = false;
            bool enabled = true;
            std::vector<MenuItem> submenu;
        };

        struct MenuConfig {
            std::vector<std::pair<std::string, std::string>> availableThemes;
            std::vector<std::string> availableProfiles;
            std::string currentTheme;
            std::string currentProfile;
            bool isPaused = false;
            bool isRunning = true;
        };

        using ActionCallback = std::function<void(const std::string& actionId)>;

        WallpaperTrayMenu();
        void setActionCallback(ActionCallback cb) { m_actionCb = std::move(cb); }

        void updateMenuState(const MenuConfig& config);
        std::vector<MenuItem> buildMenu() const;
        void handleSelection(const std::string& actionId);

    private:
        ActionCallback m_actionCb;
        MenuConfig m_currentConfig;
    };

} // namespace ShapeEngine::Wallpaper
