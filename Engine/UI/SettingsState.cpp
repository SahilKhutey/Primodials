#include "SettingsState.h"
#include "Core/Logger.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>

namespace ShapeEngine::UI {

    namespace fs = std::filesystem;

    bool SettingsState::save(const std::string& path) const {
        fs::path p(path);
        if (p.has_parent_path() && !p.parent_path().empty()) {
            fs::create_directories(p.parent_path());
        }
        std::ofstream f(path);
        if (!f.is_open()) {
            SHAPE_LOG_ERROR("Failed to open file for saving settings: {}", path);
            return false;
        }

        f << "{\n";
        f << "  \"currentThemeId\": \"" << currentThemeId << "\",\n";
        f << "  \"currentProfileName\": \"" << currentProfileName << "\",\n";
        f << "  \"fpsTarget\": " << fpsTarget << ",\n";
        f << "  \"respectBattery\": " << (respectBattery ? "true" : "false") << ",\n";
        f << "  \"pauseWhenFullscreen\": " << (pauseWhenFullscreen ? "true" : "false") << ",\n";
        f << "  \"pauseWhenIdle\": " << (pauseWhenIdle ? "true" : "false") << ",\n";
        f << "  \"idleThresholdMinutes\": " << idleThresholdMinutes << ",\n";
        f << "  \"glowIntensityOverride\": " << glowIntensityOverride << ",\n";
        f << "  \"particleDensityOverride\": " << particleDensityOverride << ",\n";
        f << "  \"motionSpeedOverride\": " << motionSpeedOverride << ",\n";
        f << "  \"populationDensityOverride\": " << populationDensityOverride << ",\n";
        f << "  \"vignetteIntensityOverride\": " << vignetteIntensityOverride << ",\n";
        f << "  \"useCustomColors\": " << (useCustomColors ? "true" : "false") << ",\n";
        f << "  \"cameraAutoPan\": " << (cameraAutoPan ? "true" : "false") << ",\n";
        f << "  \"cameraDriftSpeed\": " << cameraDriftSpeed << ",\n";
        f << "  \"audioEnabled\": " << (audioEnabled ? "true" : "false") << ",\n";
        f << "  \"audioVolume\": " << audioVolume << ",\n";
        f << "  \"particlesEnabled\": " << (particlesEnabled ? "true" : "false") << ",\n";
        f << "  \"autoStartWithSystem\": " << (autoStartWithSystem ? "true" : "false") << ",\n";
        f << "  \"startMinimized\": " << (startMinimized ? "true" : "false") << ",\n";
        f << "  \"closeToTray\": " << (closeToTray ? "true" : "false") << "\n";
        f << "}\n";

        SHAPE_LOG_INFO("Settings saved to {}", path);
        return true;
    }

    bool SettingsState::load(const std::string& path) {
        std::ifstream f(path);
        if (!f.is_open()) return false;

        std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        if (content.find("coral-reef") != std::string::npos) {
            currentThemeId = "coral-reef";
        } else if (content.find("aurora") != std::string::npos) {
            currentThemeId = "aurora";
        } else if (content.find("deep-sea") != std::string::npos) {
            currentThemeId = "deep-sea";
        }

        auto pos = content.find("\"fpsTarget\":");
        if (pos != std::string::npos) {
            fpsTarget = std::atoi(content.c_str() + pos + 12);
        }

        pos = content.find("\"audioVolume\":");
        if (pos != std::string::npos) {
            audioVolume = static_cast<float>(std::atof(content.c_str() + pos + 14));
        }

        pos = content.find("\"particleDensityOverride\":");
        if (pos != std::string::npos) {
            particleDensityOverride = static_cast<float>(std::atof(content.c_str() + pos + 26));
        }

        SHAPE_LOG_INFO("Settings loaded from {}", path);
        return true;
    }

    bool SettingsState::saveProfile(const std::string& profileName) {
        m_profilesDirectory = "Content/_user/profiles";
        fs::create_directories(m_profilesDirectory);
        std::string path = m_profilesDirectory + "/" + profileName + ".json";
        if (save(path)) {
            currentProfileName = profileName;
            if (std::find(availableProfileNames.begin(), 
                          availableProfileNames.end(), profileName) 
                == availableProfileNames.end()) {
                availableProfileNames.push_back(profileName);
            }
            return true;
        }
        return false;
    }

    bool SettingsState::loadProfile(const std::string& profileName) {
        std::string path = m_profilesDirectory + "/" + profileName + ".json";
        if (load(path)) {
            currentProfileName = profileName;
            notifyChanged("profile");
            return true;
        }
        return false;
    }

    std::vector<std::string> SettingsState::listProfiles() const {
        std::vector<std::string> profiles;
        if (fs::exists(m_profilesDirectory)) {
            for (const auto& entry : fs::directory_iterator(m_profilesDirectory)) {
                if (entry.path().extension() == ".json") {
                    profiles.push_back(entry.path().stem().string());
                }
            }
        }
        return profiles;
    }

    bool SettingsState::deleteProfile(const std::string& profileName) {
        std::string path = m_profilesDirectory + "/" + profileName + ".json";
        std::error_code ec;
        return fs::remove(path, ec);
    }

} // namespace ShapeEngine::UI
