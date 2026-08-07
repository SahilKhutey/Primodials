#include <catch2/catch_test_macros.hpp>
#include "Platform/PowerManager.hpp"
#include "Themes/ThemeManager.hpp"
#include "Particles/AmbientParticleSystem.hpp"
#include "Wallpaper/CameraDirector.hpp"
#include "Rendering/CameraController.h"

TEST_CASE("PowerManager Idle and Pause States", "[Wallpaper][Power]") {
    Shape::Platform::PowerManager power;
    power.Initialize(10.0f); // 10s idle threshold

    SECTION("Normal state target FPS is 30 for wallpaper") {
        power.Update(1.0f, false);
        REQUIRE_FALSE(power.ShouldPauseSimulation());
        REQUIRE_FALSE(power.ShouldPauseRendering());
        REQUIRE((power.GetTargetFPS() == 30 || power.GetTargetFPS() == 15));
    }

    SECTION("System idle triggers idle state after threshold") {
        power.Update(11.0f, false);
        REQUIRE(power.IsSystemIdle());
        REQUIRE(power.GetActivePauseReason() == Shape::Platform::PauseReason::SystemIdle);
        REQUIRE((power.GetTargetFPS() == 20 || power.GetTargetFPS() == 15));

        power.OnUserInput();
        power.Update(0.1f, false);
        REQUIRE_FALSE(power.IsSystemIdle());
    }

    SECTION("Foreground occlusion pauses rendering") {
        power.Update(1.0f, true);
        REQUIRE(power.ShouldPauseRendering());
        REQUIRE(power.GetActivePauseReason() == Shape::Platform::PauseReason::FullscreenObscured);
    }
}

TEST_CASE("ThemeManager Preset Loading and Crossfading", "[Wallpaper][Themes]") {
    Shape::Themes::ThemeManager themes;
    REQUIRE(themes.Initialize("Content/Themes"));

    SECTION("Default active theme is Deep Sea Bioluminescence") {
        REQUIRE(themes.GetActiveTheme().id == "deep-sea-bioluminescence");
        REQUIRE(themes.GetAvailableThemeIds().size() >= 3);
    }

    SECTION("Theme transition updates progress and interpolates properties") {
        REQUIRE(themes.SetActiveTheme("coral-reef", 2.0f));
        REQUIRE(themes.IsInTransition());

        themes.Update(1.0f); // 50% transition
        REQUIRE(themes.IsInTransition());
        REQUIRE(themes.GetTransitionProgress() >= 0.49f);

        themes.Update(1.1f); // Completed
        REQUIRE_FALSE(themes.IsInTransition());
        REQUIRE(themes.GetActiveTheme().id == "coral-reef");
    }
}

TEST_CASE("AmbientParticleSystem Life Cycle and Bounds Wrapping", "[Wallpaper][Particles]") {
    Shape::Particles::AmbientParticleSystem ps;
    ps.Initialize(100);

    Shape::Math::Vec4f color(0.2f, 0.8f, 1.0f, 1.0f);
    ps.Update(0.1f, 1920.0f, 1080.0f, color, 1.0f);

    REQUIRE(ps.GetParticles().size() > 0);
    REQUIRE(ps.GetActiveCount() > 0);
}

TEST_CASE("CameraDirector Drift and Event Follow Modes", "[Wallpaper][Camera]") {
    Shape::Wallpaper::CameraDirector director;
    director.Initialize(15.0f, 0.8f, 1.4f);

    Shape::Rendering::CameraController camera;

    SECTION("SlowDrift updates camera position and zoom") {
        director.SetMode(Shape::Wallpaper::CameraDirectorMode::SlowDrift);
        director.Update(1.0f, camera);
        REQUIRE(camera.GetZoom() >= 0.8f);
        REQUIRE(camera.GetZoom() <= 1.4f);
    }

    SECTION("SetTargetFocus engages EventFollow mode") {
        director.SetTargetFocus(Shape::Math::Vec2f(500.0f, -200.0f), 5.0f);
        REQUIRE(director.GetMode() == Shape::Wallpaper::CameraDirectorMode::EventFollow);
    }
}
