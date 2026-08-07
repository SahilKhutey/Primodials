// Engine/Wallpaper/WallpaperTrayMenu.cpp
#include "WallpaperTrayMenu.h"

namespace ShapeEngine::Wallpaper {

    WallpaperTrayMenu::WallpaperTrayMenu() = default;

    void WallpaperTrayMenu::updateMenuState(const MenuConfig& config) {
        m_currentConfig = config;
    }

    std::vector<WallpaperTrayMenu::MenuItem> WallpaperTrayMenu::buildMenu() const {
        std::vector<MenuItem> menu;
        
        MenuItem status;
        status.label = m_currentConfig.isPaused ? "Resume" : "Pause";
        status.id = m_currentConfig.isPaused ? "resume" : "pause";
        menu.push_back(status);
        
        MenuItem open;
        open.label = "Open Game View";
        open.id = "open_game";
        menu.push_back(open);
        
        menu.push_back({"", "", true, false, false, false, {}});
        
        MenuItem themeMenu;
        themeMenu.label = "Theme";
        themeMenu.id = "theme_menu";
        themeMenu.submenu.push_back({"Browse Workshop...", "browse_workshop", false, false, false, true, {}});
        themeMenu.submenu.push_back({"", "", true, false, false, false, {}});
        
        for (const auto& [themeId, themeName] : m_currentConfig.availableThemes) {
            MenuItem item;
            item.label = themeName;
            item.id = "theme:" + themeId;
            item.isCheckable = true;
            item.isChecked = (themeId == m_currentConfig.currentTheme);
            themeMenu.submenu.push_back(item);
        }
        menu.push_back(themeMenu);
        
        MenuItem speedMenu;
        speedMenu.label = "Speed";
        speedMenu.id = "speed_menu";
        speedMenu.submenu.push_back({"Slow (0.5x)", "speed:0.5", false, false, false, true, {}});
        speedMenu.submenu.push_back({"Normal (1x)", "speed:1.0", false, false, false, true, {}});
        speedMenu.submenu.push_back({"Fast (2x)", "speed:2.0", false, false, false, true, {}});
        menu.push_back(speedMenu);
        
        menu.push_back({"", "", true, false, false, false, {}});
        
        MenuItem profileMenu;
        profileMenu.label = "Profile";
        profileMenu.id = "profile_menu";
        profileMenu.submenu.push_back({"Save Current as Profile...", "save_profile", false, false, false, true, {}});
        profileMenu.submenu.push_back({"", "", true, false, false, false, {}});
        for (const auto& profile : m_currentConfig.availableProfiles) {
            MenuItem item;
            item.label = profile;
            item.id = "profile:" + profile;
            item.isCheckable = true;
            item.isChecked = (profile == m_currentConfig.currentProfile);
            profileMenu.submenu.push_back(item);
        }
        menu.push_back(profileMenu);
        
        MenuItem settings;
        settings.label = "Settings...";
        settings.id = "open_settings";
        menu.push_back(settings);
        
        MenuItem diary;
        diary.label = "Ecosystem Diary";
        diary.id = "open_diary";
        menu.push_back(diary);
        
        MenuItem screenshot;
        screenshot.label = "Take Screenshot";
        screenshot.id = "screenshot";
        menu.push_back(screenshot);
        
        menu.push_back({"", "", true, false, false, false, {}});
        
        MenuItem about;
        about.label = "About";
        about.id = "about";
        menu.push_back(about);
        
        MenuItem quit;
        quit.label = "Quit Primordials";
        quit.id = "quit";
        menu.push_back(quit);
        
        return menu;
    }

    void WallpaperTrayMenu::handleSelection(const std::string& actionId) {
        if (m_actionCb) m_actionCb(actionId);
    }

} // namespace ShapeEngine::Wallpaper
