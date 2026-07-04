#pragma once
#include "ECS/ComponentTypes.hpp"

#include "ECS/Entity.hpp"
#include "ShapeEngine/Math/Vector2.hpp"

namespace Shape {

enum class AIAction : u8 {
    Idle,
    Explore,
    Gather,
    Reproduce,
    Escape
};

struct AIComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::AIComponent);
    AIAction currentAction = AIAction::Idle;
    Entity targetEntity = INVALID_ENTITY;
    Math::Vector2f targetPosition = Math::Vector2f::Zero();
    
    f32 actionTimer = 0.0f;       // Cooldown timer between weight evaluations
    f32 evaluationInterval = 0.5f; // Evaluate decisions twice a second

    AIComponent() = default;
};

} // namespace Shape
