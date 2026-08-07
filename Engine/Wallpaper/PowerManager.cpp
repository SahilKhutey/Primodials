#include "PowerManager.h"
#include "Core/Logger.hpp"

#include <SDL3/SDL.h>
#include <algorithm>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #include <powrprof.h>
#endif

namespace ShapeEngine::Wallpaper {

    PowerManager::PowerManager() = default;
    PowerManager::~PowerManager() = default;

    void PowerManager::initialize(const Config& config) {
        m_config = config;
        m_timeSinceLastInput = 0.0f;
        m_pauseReasons = 0;

        detectPowerSource();
        detectLowPowerMode();

        SHAPE_LOG_INFO("PowerManager initialized: {} FPS on AC, {} on battery",
                 m_config.fpsOnAC, m_config.fpsOnBattery);
    }

    void PowerManager::shutdown() {
        m_pauseReasons = 0;
    }

    bool PowerManager::update(float deltaTime) {
        uint32_t oldReasons = m_pauseReasons;

        detectPowerSource();
        detectLowPowerMode();

        m_pauseReasons = 0;
        if (m_userPaused) m_pauseReasons |= (uint32_t)PauseReason::UserPaused;
        if (detectUserIdle(deltaTime)) m_pauseReasons |= (uint32_t)PauseReason::SystemIdle;
        if (m_config.respectFullscreen && detectFullscreenCovering()) 
            m_pauseReasons |= (uint32_t)PauseReason::FullscreenCovering;
        if (m_config.respectBattery && m_powerSource == PowerSource::Battery) 
            m_pauseReasons |= (uint32_t)PauseReason::OnBattery;
        if (m_inLowPowerMode) 
            m_pauseReasons |= (uint32_t)PauseReason::LowPowerMode;

        if (m_pauseReasons != oldReasons && m_stateChangeCb) {
            m_stateChangeCb(m_pauseReasons);
        }
        return m_pauseReasons != oldReasons;
    }

    void PowerManager::notifyUserInput() {
        m_timeSinceLastInput = 0.0f;

        if (m_pauseReasons & (uint32_t)PauseReason::SystemIdle) {
            m_pauseReasons &= ~(uint32_t)PauseReason::SystemIdle;
            if (m_stateChangeCb) m_stateChangeCb(m_pauseReasons);
        }
    }

    void PowerManager::setUserPaused(bool paused) {
        m_userPaused = paused;
    }

    bool PowerManager::shouldTickSimulation() const {
        if (m_pauseReasons & ((uint32_t)PauseReason::UserPaused | 
                               (uint32_t)PauseReason::FullscreenCovering |
                               (uint32_t)PauseReason::OffHours)) {
            return false;
        }
        return true;
    }

    bool PowerManager::shouldRender() const {
        if (m_pauseReasons & (uint32_t)PauseReason::FullscreenCovering) {
            return false;
        }
        return true;
    }

    int PowerManager::getTargetFPS() const {
        if (m_pauseReasons & (uint32_t)PauseReason::FullscreenCovering) {
            return m_config.fpsWhenCovered;
        }
        if (m_powerSource == PowerSource::Battery) {
            return m_config.fpsOnBattery;
        }
        if (m_inLowPowerMode) {
            return std::min(m_config.fpsOnAC, 20);
        }
        return m_config.fpsOnAC;
    }

    PowerManager::PowerSource PowerManager::getPowerSource() const {
        return m_powerSource;
    }

    void PowerManager::detectPowerSource() {
#ifdef _WIN32
        SYSTEM_POWER_STATUS status;
        if (GetSystemPowerStatus(&status)) {
            if (status.ACLineStatus == 1) m_powerSource = PowerSource::AC;
            else if (status.ACLineStatus == 0) m_powerSource = PowerSource::Battery;
            else m_powerSource = PowerSource::Unknown;
        }
#else
        m_powerSource = PowerSource::AC;
#endif
    }

    void PowerManager::detectLowPowerMode() {
#ifdef _WIN32
        GUID* pPwrScheme = nullptr;
        if (PowerGetActiveScheme(nullptr, &pPwrScheme) == ERROR_SUCCESS && pPwrScheme) {
            m_inLowPowerMode = (memcmp(pPwrScheme, &GUID_MAX_POWER_SAVINGS, sizeof(GUID)) == 0);
            LocalFree(pPwrScheme);
        }
#else
        m_inLowPowerMode = false;
#endif
    }

    bool PowerManager::detectFullscreenCovering() {
#ifdef _WIN32
        HWND foreground = GetForegroundWindow();
        if (!foreground) return false;

        HMONITOR ourMonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
        HMONITOR fgMonitor = MonitorFromWindow(foreground, MONITOR_DEFAULTTONEAREST);

        if (ourMonitor != fgMonitor) return false;

        RECT rc;
        GetWindowRect(foreground, &rc);
        MONITORINFO mi{};
        mi.cbSize = sizeof(mi);
        GetMonitorInfoW(ourMonitor, &mi);

        return (rc.left <= mi.rcMonitor.left && rc.top <= mi.rcMonitor.top &&
                rc.right >= mi.rcMonitor.right && rc.bottom >= mi.rcMonitor.bottom);
#else
        return false;
#endif
    }

    bool PowerManager::detectUserIdle(float dt) {
        m_timeSinceLastInput += dt;
        return m_timeSinceLastInput >= m_config.idleThresholdSeconds;
    }

} // namespace ShapeEngine::Wallpaper
