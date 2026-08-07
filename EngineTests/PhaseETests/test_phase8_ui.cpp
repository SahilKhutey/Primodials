// EngineTests/PhaseETests/test_phase8_ui.cpp
#include <catch2/catch_test_macros.hpp>
#include "UI/ThemeSystem/ThemeSelector.h"
#include "UI/Settings/SettingsWindow.h"
#include "Wallpaper/WallpaperTrayMenu.h"

TEST_CASE("ThemeSelector initialization and theme selection", "[phase8]") {
    ShapeEngine::UI::ThemeSelector::Config cfg;
    cfg.window = nullptr;
    cfg.renderer = nullptr;
    cfg.themesDirectory = "Content/Themes";

    ShapeEngine::UI::ThemeSelector selector(cfg);
    REQUIRE_FALSE(selector.isVisible());
    selector.show();
    REQUIRE(selector.isVisible());

    selector.setSelectedTheme("aurora");
    REQUIRE(selector.getSelectedTheme() == "aurora");
    selector.update(0.1f);
    selector.hide();
    REQUIRE_FALSE(selector.isVisible());
}

TEST_CASE("SettingsWindow value mutation and JSON serialization", "[phase8]") {
    ShapeEngine::UI::SettingsWindow::Config cfg;
    cfg.window = nullptr;
    cfg.renderer = nullptr;

    ShapeEngine::UI::SettingsWindow settings(cfg);
    REQUIRE(settings.get("performance.target_fps").intVal == 30);

    settings.set("performance.target_fps", ShapeEngine::UI::SettingsWindow::SettingValue{
        ShapeEngine::UI::SettingsWindow::SettingValue::Int, false, 60
    });
    REQUIRE(settings.get("performance.target_fps").intVal == 60);

    const std::string savePath = "test_phase8_settings.json";
    REQUIRE(settings.save(savePath));

    ShapeEngine::UI::SettingsWindow loadedSettings(cfg);
    REQUIRE(loadedSettings.load(savePath));
    REQUIRE(loadedSettings.get("performance.target_fps").intVal == 60);
}

TEST_CASE("WallpaperTrayMenu menu building and action callback", "[phase8]") {
    ShapeEngine::Wallpaper::WallpaperTrayMenu trayMenu;
    
    ShapeEngine::Wallpaper::WallpaperTrayMenu::MenuConfig cfg;
    cfg.availableThemes = {{"aurora", "Aurora"}, {"deep-sea", "Deep Sea"}};
    cfg.currentTheme = "aurora";
    cfg.availableProfiles = {"Default", "Night"};
    cfg.currentProfile = "Default";
    cfg.isPaused = false;

    trayMenu.updateMenuState(cfg);
    auto menu = trayMenu.buildMenu();
    REQUIRE_FALSE(menu.empty());

    bool actionTriggered = false;
    std::string triggeredAction;
    trayMenu.setActionCallback([&](const std::string& actionId) {
        actionTriggered = true;
        triggeredAction = actionId;
    });

    trayMenu.handleSelection("theme:deep-sea");
    REQUIRE(actionTriggered);
    REQUIRE(triggeredAction == "theme:deep-sea");
}
