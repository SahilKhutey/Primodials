// EngineTests/PhaseETests/test_ui_system.cpp
#include <catch2/catch_test_macros.hpp>
#include "UI/Loading/LoadingScreen.h"
#include "UI/Menus/MainMenu.h"
#include "UI/Menus/PauseMenu.h"
#include "UI/HUD/GameHUD.h"

TEST_CASE("LoadingScreen state and progress transitions", "[ui]") {
    ShapeEngine::UI::LoadingScreen::Config cfg;
    cfg.window = nullptr;
    cfg.renderer = nullptr;
    
    ShapeEngine::UI::LoadingScreen loading(cfg);
    
    REQUIRE_FALSE(loading.isLoading());
    loading.startLoading("Initializing Primordials...");
    REQUIRE(loading.isLoading());
    
    loading.setProgress(0.5f);
    loading.setState(ShapeEngine::UI::LoadingState::LoadingAssets, "Loading textures...");
    loading.update(0.1f);
    
    loading.finishLoading();
    loading.update(0.5f);
    REQUIRE_FALSE(loading.isLoading());
}

TEST_CASE("MainMenu configuration and actions", "[ui]") {
    ShapeEngine::UI::MainMenu::Config cfg;
    cfg.window = nullptr;
    cfg.renderer = nullptr;
    
    ShapeEngine::UI::MainMenu menu(cfg);
    
    REQUIRE(menu.isVisible());
    menu.setHoveredAction(ShapeEngine::UI::MainMenu::MenuAction::NewGame);
    REQUIRE(menu.getHoveredAction() == ShapeEngine::UI::MainMenu::MenuAction::NewGame);
    
    menu.update(0.016f);
    menu.toggle();
    REQUIRE_FALSE(menu.isVisible());
}

TEST_CASE("PauseMenu visibility and toggle", "[ui]") {
    ShapeEngine::UI::PauseMenu pauseMenu(nullptr, nullptr);
    
    REQUIRE_FALSE(pauseMenu.isVisible());
    pauseMenu.show();
    REQUIRE(pauseMenu.isVisible());
    pauseMenu.toggle();
    REQUIRE_FALSE(pauseMenu.isVisible());
}

TEST_CASE("GameHUD state and notifications", "[ui]") {
    ShapeEngine::UI::GameHUD hud(nullptr, nullptr);
    
    REQUIRE(hud.isVisible());
    hud.setEntityCount(250000);
    hud.setSimulationSpeed(2.0f);
    hud.setCurrentTick(1000);
    hud.setFPS(60.0f);
    
    hud.addNotification("Ecosystem stable", 2.0f);
    hud.update(0.5f);
}
