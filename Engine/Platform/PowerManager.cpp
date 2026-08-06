#include "Platform/PowerManager.hpp"
#include <SDL3/SDL.h>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace Shape::Platform {

void PowerManager::Initialize(f32 idleThresholdSeconds) {
    m_IdleThreshold = idleThresholdSeconds;
    m_TimeSinceLastInput = 0.0f;
    m_UserPaused = false;
    m_PauseReason = PauseReason::None;
}

void PowerManager::Update(f32 dt, bool isObscuredByForeground) {
    m_TimeSinceLastInput += dt;
    m_IsObscured = isObscuredByForeground;

    if (m_UserPaused) {
        m_PauseReason = PauseReason::UserPaused;
    } else if (m_IsObscured) {
        m_PauseReason = PauseReason::FullscreenObscured;
    } else if (IsSystemIdle()) {
        m_PauseReason = PauseReason::SystemIdle;
    } else {
        m_PauseReason = PauseReason::None;
    }
}

void PowerManager::OnUserInput() {
    m_TimeSinceLastInput = 0.0f;
}

bool PowerManager::ShouldPauseSimulation() const {
    return m_PauseReason == PauseReason::UserPaused;
}

bool PowerManager::ShouldPauseRendering() const {
    return m_PauseReason == PauseReason::FullscreenObscured;
}

i32 PowerManager::GetTargetFPS() const {
    if (ShouldPauseRendering()) {
        return 1; // Sleep loop when completely obscured
    }
    if (IsOnBattery()) {
        return 15;
    }
    if (IsSystemIdle()) {
        return 20;
    }
    return 30; // Wallpaper standard frame rate target
}

bool PowerManager::IsOnBattery() const {
#ifdef _WIN32
    SYSTEM_POWER_STATUS status;
    if (GetSystemPowerStatus(&status)) {
        return (status.ACLineStatus == 0); // 0 = Offline (on battery)
    }
#endif
    return false;
}

} // namespace Shape::Platform
