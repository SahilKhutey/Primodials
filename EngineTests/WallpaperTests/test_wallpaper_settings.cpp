#include <catch2/catch_test_macros.hpp>
#include "Audio/AmbientPlayer.hpp"
#include "Wallpaper/WallpaperSettings.hpp"
#include <fstream>

TEST_CASE("AmbientPlayer Crossfading and Volume Control", "[Wallpaper][Audio]") {
    Shape::Audio::AmbientPlayer player;
    REQUIRE(player.Initialize());

    SECTION("Master volume bounds clamping") {
        player.SetMasterVolume(1.5f);
        REQUIRE(player.GetMasterVolume() == 1.0f);

        player.SetMasterVolume(-0.5f);
        REQUIRE(player.GetMasterVolume() == 0.0f);
    }

    SECTION("PlayThemeAudio initiates crossfade and completes over duration") {
        player.PlayThemeAudio("audio/ocean-ambient.ogg", 0.4f, 2.0f);
        REQUIRE_FALSE(player.IsPlaying()); // Transitions in

        player.Update(1.0f); // 50% crossfade
        REQUIRE(player.GetCurrentAudioPath() == "");

        player.Update(1.1f); // Finished
        REQUIRE(player.IsPlaying());
        REQUIRE(player.GetCurrentAudioPath() == "audio/ocean-ambient.ogg");
    }
}

TEST_CASE("WallpaperSettings Save and Load Round-Trip", "[Wallpaper][Settings]") {
    Shape::Wallpaper::WallpaperSettings settings;
    settings.SetDefaults();

    settings.GetPreferences().activeThemeId = "forest-floor";
    settings.GetPreferences().targetFpsCap = 60;
    settings.GetPreferences().masterAudioVolume = 0.75f;

    std::string testPath = "test_wallpaper_settings.json";
    REQUIRE(settings.SaveToFile(testPath));

    Shape::Wallpaper::WallpaperSettings loadedSettings;
    REQUIRE(loadedSettings.LoadFromFile(testPath));
    REQUIRE(loadedSettings.GetPreferences().activeThemeId == "forest-floor");

    std::remove(testPath.c_str());
}
