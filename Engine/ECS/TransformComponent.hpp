#pragma once

#include "ShapeEngine/Math/Vector2.hpp"

namespace Shape {

struct TransformComponent {
    Math::Vector2f position = {0.0f, 0.0f};
    f32 rotation = 0.0f; // radians
    Math::Vector2f scale = {1.0f, 1.0f};
    Math::Vector2f previousPosition = {0.0f, 0.0f};
    f32 previousRotation = 0.0f;
};

} // namespace Shape
