#pragma once

#include "Core/Platform.hpp"
#include <string>

namespace Shape::Wallpaper {

struct WallpaperPreferences {
    std::string activeThemeId = "deep-sea-bioluminescence";
    i32 targetFpsCap = 30;
    f32 glowIntensityMultiplier = 1.0f;
    f32 particleDensityMultiplier = 1.0f;
    f32 masterAudioVolume = 0.5f;
    bool autoStartWithOS = false;
    bool enableClickThrough = true;
};

class WallpaperSettings {
public:
    WallpaperSettings() = default;
    ~WallpaperSettings() = default;

    bool LoadFromFile(const std::string& jsonPath = "Saves/wallpaper_settings.json");
    bool SaveToFile(const std::string& jsonPath = "Saves/wallpaper_settings.json") const;

    WallpaperPreferences& GetPreferences() { return m_Prefs; }
    const WallpaperPreferences& GetPreferences() const { return m_Prefs; }

    void SetDefaults();

private:
    WallpaperPreferences m_Prefs;
};

} // namespace Shape::Wallpaper
