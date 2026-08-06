#pragma once

#include "Core/Platform.hpp"
#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include "ShapeEngine/Math/Vector2.hpp"
#include <vector>

namespace Shape::UI {

class EntityPicker {
public:
    static Entity PickEntityAtPosition(World& world, const Math::Vector2f& worldPos, f32 pickRadius = 25.0f) {
        auto entities = world.Query<TransformComponent>();
        Entity closestEntity = INVALID_ENTITY;
        f32 closestDistSq = pickRadius * pickRadius;

        for (Entity e : entities) {
            const auto& tf = world.GetComponent<TransformComponent>(e);
            f32 dx = tf.position.x - worldPos.x;
            f32 dy = tf.position.y - worldPos.y;
            f32 distSq = dx * dx + dy * dy;

            if (distSq <= closestDistSq) {
                closestDistSq = distSq;
                closestEntity = e;
            }
        }
        return closestEntity;
    }
};

} // namespace Shape::UI
