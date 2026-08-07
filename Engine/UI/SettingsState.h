#pragma once

#include "Themes/Theme.h"
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

namespace ShapeEngine::UI {

    class SettingsState {
    public:
        std::string currentThemeId = "deep-sea";
        std::string currentProfileName = "Default";
        std::vector<std::string> availableProfileNames = { "Default" };

        int  fpsTarget = 30;
        bool respectBattery = true;
        bool pauseWhenFullscreen = true;
        bool pauseWhenIdle = true;
        float idleThresholdMinutes = 5.0f;

        float glowIntensityOverride = 1.0f;
        float particleDensityOverride = 1.0f;
        float motionSpeedOverride = 1.0f;
        float populationDensityOverride = 1.0f;
        float vignetteIntensityOverride = 1.0f;

        bool  useCustomColors = false;
        float customBackgroundTop[4]    = { 0.02f, 0.03f, 0.08f, 1.0f };
        float customBackgroundBottom[4] = { 0.0f, 0.01f, 0.04f, 1.0f };
        float customAccentColor[4]      = { 0.4f, 0.95f, 1.0f, 1.0f };

        bool  cameraAutoPan = true;
        float cameraDriftSpeed = 20.0f;
        float cameraZoomMin = 0.8f;
        float cameraZoomMax = 1.3f;
        bool  cameraFollowEvents = true;

        bool  audioEnabled = true;
        float audioVolume = 0.25f;
        bool  audioMuteWhenFullscreen = true;
        std::string customAudioPath;

        bool  particlesEnabled = true;
        std::vector<std::string> enabledParticleTypes = { "bubble", "mote" };

        bool  autoStartWithSystem = false;
        bool  startMinimized = true;
        bool  closeToTray = true;

        bool  showNotifications = true;
        bool  notifyOnExtinction = true;
        bool  notifyOnNewSpecies = false;
        bool  notifyOnDailyMilestone = true;

        static SettingsState& get() {
            static SettingsState instance;
            return instance;
        }

        using ChangeCallback = std::function<void(const std::string& key)>;
        void setOnChange(ChangeCallback cb) { m_changeCb = std::move(cb); }

        void notifyChanged(const std::string& key) {
            if (m_changeCb) m_changeCb(key);
        }

        bool save(const std::string& path) const;
        bool load(const std::string& path);

        bool saveProfile(const std::string& profileName);
        bool loadProfile(const std::string& profileName);
        std::vector<std::string> listProfiles() const;
        bool deleteProfile(const std::string& profileName);

    private:
        SettingsState() = default;
        ChangeCallback m_changeCb;
        std::string m_profilesDirectory = "Content/_user/profiles";
    };

} // namespace ShapeEngine::UI
