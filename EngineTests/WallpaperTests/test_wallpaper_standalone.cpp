#include <catch2/catch_test_macros.hpp>
#include "Wallpaper/WallpaperWindow.h"
#include "Wallpaper/PowerManager.h"
#include "Themes/ThemeManager.h"

using namespace ShapeEngine;
using namespace ShapeEngine::Wallpaper;

TEST_CASE("Wallpaper window initializes in WindowedNormal mode", "[wallpaper]") {
    WallpaperWindow window;
    REQUIRE(window.initialize(WallpaperWindow::Mode::WindowedNormal, 1920, 1080));
    REQUIRE(window.getSDLWindow() != nullptr);
    window.shutdown();
}

TEST_CASE("Power manager detects pause reasons and idle thresholds", "[wallpaper][power]") {
    PowerManager pm;
    PowerManager::Config cfg;
    pm.initialize(cfg);

    pm.setUserPaused(true);
    pm.update(0.1f);
    REQUIRE(pm.shouldTickSimulation() == false);

    pm.setUserPaused(false);
    pm.update(0.1f);
    REQUIRE(pm.shouldTickSimulation() == true);
    pm.shutdown();
}

TEST_CASE("Theme manager loads themes from disk and interpolates colors", "[wallpaper][themes]") {
    Themes::ThemeManager tm;
    REQUIRE(tm.initialize("Content/Themes"));

    auto themes = tm.discoverThemes();
    REQUIRE(themes.size() >= 3);

    bool foundDeepSea = false, foundCoral = false, foundAurora = false;
    for (const auto& t : themes) {
        if (t.id == "deep-sea") foundDeepSea = true;
        if (t.id == "coral-reef") foundCoral = true;
        if (t.id == "aurora") foundAurora = true;
    }
    REQUIRE(foundDeepSea);
    REQUIRE(foundCoral);
    REQUIRE(foundAurora);

    auto deepSea = tm.loadTheme("deep-sea");
    auto coral = tm.loadTheme("coral-reef");
    REQUIRE(deepSea != nullptr);
    REQUIRE(coral != nullptr);

    tm.startTransition(*deepSea, *coral, 1.0f);
    for (int i = 0; i < 30; ++i) {
        tm.update(0.016f);
    }

    auto mid = tm.getCurrentInterpolatedTheme();
    REQUIRE(mid.backgroundTopColor.x >= deepSea->backgroundTopColor.x - 0.01f);
    REQUIRE(mid.backgroundTopColor.x <= coral->backgroundTopColor.x + 0.01f);
    tm.shutdown();
}
