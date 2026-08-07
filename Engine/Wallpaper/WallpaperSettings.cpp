#include "Wallpaper/WallpaperSettings.hpp"
#include "Core/Logger.hpp"
#include <fstream>
#include <sstream>

namespace Shape::Wallpaper {

void WallpaperSettings::SetDefaults() {
    m_Prefs = WallpaperPreferences{};
}

bool WallpaperSettings::LoadFromFile(const std::string& jsonPath) {
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        SHAPE_LOG_INFO("WallpaperSettings: Config file not found at '{}'. Using default preferences.", jsonPath);
        SetDefaults();
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    // Simple key-value text extraction fallback
    if (content.find("deep-sea") != std::string::npos) {
        m_Prefs.activeThemeId = "deep-sea-bioluminescence";
    } else if (content.find("coral-reef") != std::string::npos) {
        m_Prefs.activeThemeId = "coral-reef";
    } else if (content.find("aurora") != std::string::npos) {
        m_Prefs.activeThemeId = "aurora";
    } else if (content.find("microorganisms") != std::string::npos) {
        m_Prefs.activeThemeId = "microorganisms";
    } else if (content.find("forest-floor") != std::string::npos) {
        m_Prefs.activeThemeId = "forest-floor";
    }

    SHAPE_LOG_INFO("WallpaperSettings: Loaded preferences from '{}'. Active Theme: '{}'", jsonPath, m_Prefs.activeThemeId);
    return true;
}

bool WallpaperSettings::SaveToFile(const std::string& jsonPath) const {
    std::ofstream file(jsonPath);
    if (!file.is_open()) {
        SHAPE_LOG_WARN("WallpaperSettings: Could not open file for writing at '{}'", jsonPath);
        return false;
    }

    file << "{\n";
    file << "  \"activeThemeId\": \"" << m_Prefs.activeThemeId << "\",\n";
    file << "  \"targetFpsCap\": " << m_Prefs.targetFpsCap << ",\n";
    file << "  \"glowIntensityMultiplier\": " << m_Prefs.glowIntensityMultiplier << ",\n";
    file << "  \"particleDensityMultiplier\": " << m_Prefs.particleDensityMultiplier << ",\n";
    file << "  \"masterAudioVolume\": " << m_Prefs.masterAudioVolume << ",\n";
    file << "  \"autoStartWithOS\": " << (m_Prefs.autoStartWithOS ? "true" : "false") << ",\n";
    file << "  \"enableClickThrough\": " << (m_Prefs.enableClickThrough ? "true" : "false") << "\n";
    file << "}\n";

    SHAPE_LOG_INFO("WallpaperSettings: Saved preferences to '{}'", jsonPath);
    return true;
}

} // namespace Shape::Wallpaper
