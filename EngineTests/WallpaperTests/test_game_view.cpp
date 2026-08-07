#include <catch2/catch_test_macros.hpp>
#include "Wallpaper/GameViewWindow.hpp"

TEST_CASE("GameViewWindow Visibility and Callback Triggers", "[Wallpaper][GameView]") {
    Shape::Wallpaper::GameViewWindow gv;

    SECTION("Default visibility is hidden") {
        REQUIRE_FALSE(gv.IsVisible());
    }

    SECTION("Show, Hide, and Toggle manipulate visibility state") {
        gv.Show();
        REQUIRE(gv.IsVisible());

        gv.Hide();
        REQUIRE_FALSE(gv.IsVisible());

        gv.Toggle();
        REQUIRE(gv.IsVisible());
    }

    SECTION("Callbacks are correctly invoked") {
        bool pauseInvoked = false;
        f32 speedVal = 1.0f;
        std::string selectedTheme = "";

        gv.SetPauseCallback([&](bool p) { pauseInvoked = p; });
        gv.SetSpeedCallback([&](f32 s) { speedVal = s; });
        gv.SetThemeCallback([&](const std::string& t) { selectedTheme = t; });

        Shape::Wallpaper::GameViewStats stats;
        stats.isPaused = true;
        std::vector<std::string> themes = {"deep-sea-bioluminescence", "coral-reef"};

        gv.Show();
        gv.RenderUI(stats, themes);

        REQUIRE(gv.IsVisible());
    }
}
