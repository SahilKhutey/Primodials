#include "Platform/SystemTray.hpp"
#include "Core/Logger.hpp"
#include <SDL3/SDL.h>

namespace Shape::Platform {

SystemTray::~SystemTray() {
    Shutdown();
}

bool SystemTray::Initialize(const std::string& appName, const std::string& iconPath) {
    if (m_IsInitialized) {
        return true;
    }

    SDL_Surface* iconSurface = nullptr;
    if (!iconPath.empty()) {
        iconSurface = SDL_LoadBMP(iconPath.c_str());
    }

    m_Tray = SDL_CreateTray(iconSurface, appName.c_str());
    if (iconSurface) {
        SDL_DestroySurface(iconSurface);
    }

    if (!m_Tray) {
        SHAPE_LOG_WARN("SDL_CreateTray returned null: {}. System tray disabled.", SDL_GetError());
        return false;
    }

    m_Menu = SDL_CreateTrayMenu(m_Tray);
    if (!m_Menu) {
        SHAPE_LOG_WARN("SDL_CreateTrayMenu failed: {}", SDL_GetError());
        SDL_DestroyTray(m_Tray);
        m_Tray = nullptr;
        return false;
    }

    // Open Game View
    SDL_TrayEntry* openGameEntry = SDL_InsertTrayEntryAt(m_Menu, -1, "Open Game View", SDL_TRAYENTRY_BUTTON);
    SDL_SetTrayEntryCallback(openGameEntry, [](void* userdata, SDL_TrayEntry*) {
        auto* self = static_cast<SystemTray*>(userdata);
        if (self && self->m_Callback) self->m_Callback(TrayMenuItem::OpenGameView, "");
    }, this);

    // Toggle Pause
    m_PauseEntry = SDL_InsertTrayEntryAt(m_Menu, -1, "Pause Simulation", SDL_TRAYENTRY_BUTTON);
    SDL_SetTrayEntryCallback(m_PauseEntry, [](void* userdata, SDL_TrayEntry*) {
        auto* self = static_cast<SystemTray*>(userdata);
        if (self && self->m_Callback) self->m_Callback(TrayMenuItem::TogglePause, "");
    }, this);

    // Speed Submenu
    SDL_TrayEntry* speedParent = SDL_InsertTrayEntryAt(m_Menu, -1, "Speed", SDL_TRAYENTRY_SUBMENU);
    SDL_TrayMenu* speedMenu = SDL_CreateTraySubmenu(speedParent);
    if (speedMenu) {
        SDL_TrayEntry* slowEntry = SDL_InsertTrayEntryAt(speedMenu, -1, "Slow (0.5x)", SDL_TRAYENTRY_BUTTON);
        SDL_SetTrayEntryCallback(slowEntry, [](void* userdata, SDL_TrayEntry*) {
            auto* self = static_cast<SystemTray*>(userdata);
            if (self && self->m_Callback) self->m_Callback(TrayMenuItem::SpeedSlow, "");
        }, this);

        SDL_TrayEntry* normalEntry = SDL_InsertTrayEntryAt(speedMenu, -1, "Normal (1.0x)", SDL_TRAYENTRY_BUTTON);
        SDL_SetTrayEntryCallback(normalEntry, [](void* userdata, SDL_TrayEntry*) {
            auto* self = static_cast<SystemTray*>(userdata);
            if (self && self->m_Callback) self->m_Callback(TrayMenuItem::SpeedNormal, "");
        }, this);

        SDL_TrayEntry* fastEntry = SDL_InsertTrayEntryAt(speedMenu, -1, "Fast (2.0x)", SDL_TRAYENTRY_BUTTON);
        SDL_SetTrayEntryCallback(fastEntry, [](void* userdata, SDL_TrayEntry*) {
            auto* self = static_cast<SystemTray*>(userdata);
            if (self && self->m_Callback) self->m_Callback(TrayMenuItem::SpeedFast, "");
        }, this);
    }

    // Theme Submenu Parent
    SDL_TrayEntry* themeParent = SDL_InsertTrayEntryAt(m_Menu, -1, "Themes", SDL_TRAYENTRY_SUBMENU);
    m_ThemeSubmenu = SDL_CreateTraySubmenu(themeParent);

    // Settings
    SDL_TrayEntry* settingsEntry = SDL_InsertTrayEntryAt(m_Menu, -1, "Settings...", SDL_TRAYENTRY_BUTTON);
    SDL_SetTrayEntryCallback(settingsEntry, [](void* userdata, SDL_TrayEntry*) {
        auto* self = static_cast<SystemTray*>(userdata);
        if (self && self->m_Callback) self->m_Callback(TrayMenuItem::OpenSettings, "");
    }, this);

    // Quit
    SDL_TrayEntry* quitEntry = SDL_InsertTrayEntryAt(m_Menu, -1, "Quit Wallpaper", SDL_TRAYENTRY_BUTTON);
    SDL_SetTrayEntryCallback(quitEntry, [](void* userdata, SDL_TrayEntry*) {
        auto* self = static_cast<SystemTray*>(userdata);
        if (self && self->m_Callback) self->m_Callback(TrayMenuItem::Quit, "");
    }, this);

    m_IsInitialized = true;
    SHAPE_LOG_INFO("SystemTray initialized successfully.");
    return true;
}

void SystemTray::Shutdown() {
    if (m_Tray) {
        SDL_DestroyTray(m_Tray);
        m_Tray = nullptr;
        m_Menu = nullptr;
        m_PauseEntry = nullptr;
        m_ThemeSubmenu = nullptr;
    }
    m_IsInitialized = false;
}

void SystemTray::UpdateThemeList(const std::vector<std::string>& themeNames, const std::string& activeTheme) {
    if (!m_ThemeSubmenu) return;

    for (const auto& themeName : themeNames) {
        std::string label = (themeName == activeTheme) ? "● " + themeName : "○ " + themeName;
        SDL_TrayEntry* entry = SDL_InsertTrayEntryAt(m_ThemeSubmenu, -1, label.c_str(), SDL_TRAYENTRY_BUTTON);
        SDL_SetTrayEntryCallback(entry, [](void* userdata, SDL_TrayEntry* item) {
            auto* self = static_cast<SystemTray*>(userdata);
            if (self && self->m_Callback) {
                const char* itemLabel = SDL_GetTrayEntryLabel(item);
                std::string selectedName = itemLabel ? itemLabel : "";
                if (selectedName.rfind("● ", 0) == 0 || selectedName.rfind("○ ", 0) == 0) {
                    selectedName = selectedName.substr(3);
                }
                self->m_Callback(TrayMenuItem::ThemeSelect, selectedName);
            }
        }, this);
    }
}

void SystemTray::SetPaused(bool paused) {
    if (m_PauseEntry) {
        SDL_SetTrayEntryLabel(m_PauseEntry, paused ? "Resume Simulation" : "Pause Simulation");
    }
}

void SystemTray::SetTooltip(const std::string& tooltip) {
    if (m_Tray) {
        SDL_SetTrayTooltip(m_Tray, tooltip.c_str());
    }
}

} // namespace Shape::Platform
