#pragma once

#include "Core/Platform.hpp"
#include "Math/Vec2.hpp"

namespace Shape::Rendering {
    class CameraController;
}

namespace Shape::Wallpaper {

enum class CameraDirectorMode {
    Manual,       // User interactive control (Game View)
    SlowDrift,    // Automated ambient figure-8 camera drift (Wallpaper Mode)
    EventFollow   // Zoom in on ecological events or high-density clusters
};

class CameraDirector {
public:
    CameraDirector() = default;
    ~CameraDirector() = default;

    void Initialize(f32 driftSpeed = 15.0f, f32 minZoom = 0.8f, f32 maxZoom = 1.4f);
    void Update(f32 dt, Rendering::CameraController& camera);

    void SetMode(CameraDirectorMode mode) { m_Mode = mode; }
    CameraDirectorMode GetMode() const { return m_Mode; }

    void SetTargetFocus(const Math::Vec2f& worldPos, f32 holdSeconds = 8.0f);

private:
    CameraDirectorMode m_Mode = CameraDirectorMode::SlowDrift;
    f32 m_DriftSpeed = 15.0f;
    f32 m_MinZoom = 0.8f;
    f32 m_MaxZoom = 1.4f;
    f32 m_DriftPhase = 0.0f;

    Math::Vec2f m_EventTarget{0.0f, 0.0f};
    f32 m_EventHoldTimer = 0.0f;
    bool m_IsFollowingEvent = false;
};

} // namespace Shape::Wallpaper
