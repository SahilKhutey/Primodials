// Engine/Rendering/CameraController.cpp
#include "Rendering/CameraController.h"
#include "ECS/TransformComponent.hpp"
#include "Math/Matrix4.hpp"
#include <algorithm>
#include <cmath>

namespace Shape::Rendering {

Math::Vector2f CameraController::WorldToScreen(const Math::Vector2f& world_pos, const Math::Vector2f& screen_size) const {
    Math::Vector2f center = screen_size * 0.5f;
    return (world_pos - m_position) * m_zoom + center;
}

Math::Vector2f CameraController::ScreenToWorld(const Math::Vector2f& screen_pos, const Math::Vector2f& screen_size) const {
    Math::Vector2f center = screen_size * 0.5f;
    return (screen_pos - center) / (m_zoom > 0.0001f ? m_zoom : 1.0f) + m_position;
}

Math::Matrix4 CameraController::GetProjectionMatrix(f32 viewportWidth, f32 viewportHeight) const {
    f32 halfW = (viewportWidth * 0.5f) / (m_zoom > 0.0001f ? m_zoom : 1.0f);
    f32 halfH = (viewportHeight * 0.5f) / (m_zoom > 0.0001f ? m_zoom : 1.0f);
    return Math::Matrix4::Ortho(-halfW, halfW, -halfH, halfH, -1.0f, 1.0f);
}

Math::Matrix4 CameraController::GetViewMatrix() const {
    return Math::Matrix4::Translation(Math::Vector3(-m_position.x, -m_position.y, 0.0f));
}

void CameraController::Pan(const Math::Vector2f& delta_world) {
    m_target_position += delta_world;
    ClearFollowEntity();
}

void CameraController::Zoom(f32 factor, const Math::Vector2f& focus_screen_pos, const Math::Vector2f& screen_size) {
    Math::Vector2f world_focus_before = ScreenToWorld(focus_screen_pos, screen_size);
    m_target_zoom = std::clamp(m_target_zoom * factor, m_min_zoom, m_max_zoom);
    m_zoom = m_target_zoom;
    Math::Vector2f world_focus_after = ScreenToWorld(focus_screen_pos, screen_size);
    m_target_position += (world_focus_before - world_focus_after);
    m_position = m_target_position;
}

void CameraController::SetZoom(f32 zoom) {
    m_target_zoom = std::clamp(zoom, m_min_zoom, m_max_zoom);
    m_zoom = m_target_zoom;
}

void CameraController::Update(World* world, f32 dt) {
    // 1. Follow entity if active
    if (world && m_follow_entity != INVALID_ENTITY && world->IsEntityValid(m_follow_entity)) {
        if (world->HasComponent<TransformComponent>(m_follow_entity)) {
            const auto& trans = world->GetComponent<TransformComponent>(m_follow_entity);
            m_target_position = trans.position;
        }
    }

    // 2. Smooth Lerp camera position toward target
    f32 t = std::clamp(dt * m_smoothness, 0.0f, 1.0f);
    m_position = m_position * (1.0f - t) + m_target_position * t;
    m_zoom = m_zoom * (1.0f - t) + m_target_zoom * t;
}

} // namespace Shape::Rendering
