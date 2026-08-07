#pragma once
#include "ECS/ComponentTypes.hpp"

#include "ShapeEngine/Math/Vector2.hpp"

namespace Shape {

struct TransformComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::TransformComponent);
    Math::Vector2f position = {0.0f, 0.0f};
    f32 rotation = 0.0f; // radians
    Math::Vector2f scale = {1.0f, 1.0f};
    Math::Vector2f previousPosition = {0.0f, 0.0f};
    f32 previousRotation = 0.0f;
};

} // namespace Shape
