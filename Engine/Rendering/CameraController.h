// Engine/Rendering/CameraController.h
#pragma once

#include "ShapeEngine/Math/Vector2.hpp"
#include "ECS/Entity.hpp"
#include "ECS/World.hpp"
#include "Core/Platform.hpp"

namespace Shape::Rendering {

class CameraController {
public:
    CameraController() = default;
    ~CameraController() = default;

    // ── Camera Transformations ──────────────────────────────────────────────
    Math::Vector2f WorldToScreen(const Math::Vector2f& world_pos, const Math::Vector2f& screen_size) const;
    Math::Vector2f ScreenToWorld(const Math::Vector2f& screen_pos, const Math::Vector2f& screen_size) const;

    // ── Pan & Zoom Controls ────────────────────────────────────────────────
    void Pan(const Math::Vector2f& delta_world);
    void Zoom(f32 zoom_factor, const Math::Vector2f& focus_screen_pos, const Math::Vector2f& screen_size);
    void SetZoom(f32 zoom);

    void SetPosition(const Math::Vector2f& pos) { m_position = pos; m_target_position = pos; }
    Math::Vector2f GetPosition() const { return m_position; }
    f32 GetZoom() const { return m_zoom; }

    // ── Entity Tracking ────────────────────────────────────────────────────
    void FollowEntity(Entity entity) { m_follow_entity = entity; }
    void ClearFollowEntity() { m_follow_entity = INVALID_ENTITY; }
    Entity GetFollowEntity() const { return m_follow_entity; }

    // ── Frame Update ───────────────────────────────────────────────────────
    void Update(World* world, f32 dt);

    // ── Constraints ────────────────────────────────────────────────────────
    void SetZoomLimits(f32 min_zoom, f32 max_zoom) { m_min_zoom = min_zoom; m_max_zoom = max_zoom; }
    void SetSmoothing(f32 smoothness) { m_smoothness = smoothness; }

private:
    Math::Vector2f m_position{0.0f, 0.0f};
    Math::Vector2f m_target_position{0.0f, 0.0f};
    f32 m_zoom = 1.0f;
    f32 m_target_zoom = 1.0f;

    f32 m_min_zoom = 0.1f;
    f32 m_max_zoom = 10.0f;
    f32 m_smoothness = 10.0f; // Speed of lerp to target

    Entity m_follow_entity = INVALID_ENTITY;
};

} // namespace Shape::Rendering
