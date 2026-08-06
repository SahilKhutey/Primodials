#pragma once

#include "Core/Platform.hpp"

namespace Shape::Platform {

enum class PauseReason {
    None,
    UserPaused,
    SystemIdle,
    FullscreenObscured,
    BatterySaver
};

class PowerManager {
public:
    PowerManager() = default;
    ~PowerManager() = default;

    void Initialize(f32 idleThresholdSeconds = 60.0f);
    void Update(f32 dt, bool isObscuredByForeground);

    void OnUserInput();

    bool ShouldPauseSimulation() const;
    bool ShouldPauseRendering() const;
    i32 GetTargetFPS() const;

    PauseReason GetActivePauseReason() const { return m_PauseReason; }

    void SetUserPaused(bool paused) { m_UserPaused = paused; }
    bool IsUserPaused() const { return m_UserPaused; }

    bool IsSystemIdle() const { return m_TimeSinceLastInput >= m_IdleThreshold; }
    bool IsOnBattery() const;

private:
    f32 m_IdleThreshold = 60.0f;
    f32 m_TimeSinceLastInput = 0.0f;
    bool m_UserPaused = false;
    bool m_IsObscured = false;
    PauseReason m_PauseReason = PauseReason::None;
};

} // namespace Shape::Platform
