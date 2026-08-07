#include <catch2/catch_test_macros.hpp>
#include "UI/SettingsState.h"
#include "Themes/ThemeManager.h"
#include "Themes/ThemeApplier.h"
#include <cstdio>

using namespace ShapeEngine;
using namespace ShapeEngine::UI;

TEST_CASE("Settings persist across saves and loads", "[settings][persistence]") {
    auto& s = SettingsState::get();
    s.fpsTarget = 60;
    s.audioVolume = 0.5f;
    s.currentThemeId = "coral-reef";
    
    std::string testPath = "test_settings_state.json";
    REQUIRE(s.save(testPath));

    s.fpsTarget = 30;
    s.audioVolume = 0.25f;
    s.currentThemeId = "deep-sea";
    REQUIRE(s.load(testPath));

    REQUIRE(s.fpsTarget == 60);
    REQUIRE(s.currentThemeId == "coral-reef");

    std::remove(testPath.c_str());
}

TEST_CASE("Profiles save and load correctly", "[settings][profiles]") {
    auto& s = SettingsState::get();
    s.fpsTarget = 45;
    REQUIRE(s.saveProfile("TestProfile"));

    s.fpsTarget = 30;
    REQUIRE(s.loadProfile("TestProfile"));
    REQUIRE(s.fpsTarget == 45);

    REQUIRE(s.deleteProfile("TestProfile"));
}

TEST_CASE("Settings change notification fires", "[settings][observable]") {
    auto& s = SettingsState::get();
    int callCount = 0;
    std::string lastKey;

    s.setOnChange([&](const std::string& key) {
        callCount++;
        lastKey = key;
    });

    s.fpsTarget = 45;
    s.notifyChanged("fps");

    REQUIRE(callCount == 1);
    REQUIRE(lastKey == "fps");
}
