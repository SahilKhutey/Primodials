#include "Wallpaper/CameraDirector.hpp"
#include "Rendering/CameraController.h"
#include <cmath>

namespace Shape::Wallpaper {

void CameraDirector::Initialize(f32 driftSpeed, f32 minZoom, f32 maxZoom) {
    m_DriftSpeed = driftSpeed;
    m_MinZoom = minZoom;
    m_MaxZoom = maxZoom;
    m_DriftPhase = 0.0f;
    m_IsFollowingEvent = false;
    m_EventHoldTimer = 0.0f;
}

void CameraDirector::Update(f32 dt, Rendering::CameraController& camera) {
    if (m_Mode == CameraDirectorMode::Manual) {
        return; // User has full manual control in Game View
    }

    camera.SetZoomLimits(m_MinZoom, m_MaxZoom);
    camera.SetSmoothing(3.0f); // Soft ambient smoothing

    if (m_Mode == CameraDirectorMode::EventFollow && m_IsFollowingEvent) {
        m_EventHoldTimer -= dt;
        if (m_EventHoldTimer <= 0.0f) {
            m_IsFollowingEvent = false;
        } else {
            camera.SetPosition(Math::Vector2f{m_EventTarget.x, m_EventTarget.y});
            camera.SetZoom(1.3f);
            return;
        }
    }

    // Lissajous figure-8 slow ambient drift pattern
    m_DriftPhase += dt * (m_DriftSpeed * 0.005f);

    f32 driftX = std::sin(m_DriftPhase) * 150.0f;
    f32 driftY = std::sin(m_DriftPhase * 0.7f) * 80.0f;
    f32 targetZoom = m_MinZoom + (std::sin(m_DriftPhase * 0.3f) * 0.5f + 0.5f) * (m_MaxZoom - m_MinZoom);

    camera.SetPosition(Math::Vector2f{driftX, driftY});
    camera.SetZoom(targetZoom);
}

void CameraDirector::SetTargetFocus(const Math::Vec2f& worldPos, f32 holdSeconds) {
    m_EventTarget = worldPos;
    m_EventHoldTimer = holdSeconds;
    m_IsFollowingEvent = true;
    m_Mode = CameraDirectorMode::EventFollow;
}

} // namespace Shape::Wallpaper
