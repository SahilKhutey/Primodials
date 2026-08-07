#pragma once

#include <cstdint>
#include <atomic>
#include <chrono>
#include <functional>

namespace ShapeEngine::Wallpaper {

    class PowerManager {
    public:
        enum class PauseReason {
            None = 0,
            UserPaused         = 1 << 0,
            SystemIdle         = 1 << 1,
            FullscreenCovering = 1 << 2,
            OnBattery          = 1 << 3,
            LowPowerMode       = 1 << 4,
            OffHours           = 1 << 5,
        };

        struct Config {
            float   idleThresholdSeconds = 60.0f;
            bool    respectBattery = true;
            bool    respectFullscreen = true;
            int     fpsOnAC = 30;
            int     fpsOnBattery = 15;
            int     fpsWhenCovered = 5;
            bool    wakeOnUserActivity = true;
        };

        PowerManager();
        ~PowerManager();

        void initialize(const Config& config);
        void shutdown();

        bool update(float deltaTime);

        void notifyUserInput();

        void setUserPaused(bool paused);
        bool isUserPaused() const { return m_userPaused; }

        bool shouldTickSimulation() const;

        bool shouldRender() const;

        int getTargetFPS() const;

        uint32_t getActivePauseReasons() const { return m_pauseReasons; }

        enum class PowerSource { AC, Battery, Unknown };
        PowerSource getPowerSource() const;

        using StateChangeCallback = std::function<void(uint32_t activeReasons)>;
        void setOnStateChange(StateChangeCallback cb) { m_stateChangeCb = std::move(cb); }

    private:
        void detectPowerSource();
        void detectLowPowerMode();
        bool detectFullscreenCovering();
        bool detectUserIdle(float dt);

        Config m_config;

        std::atomic<bool> m_userPaused{ false };
        float m_timeSinceLastInput = 0.0f;
        uint32_t m_pauseReasons = 0;
        PowerSource m_powerSource = PowerSource::Unknown;
        bool m_inLowPowerMode = false;

        StateChangeCallback m_stateChangeCb;
    };

} // namespace ShapeEngine::Wallpaper
