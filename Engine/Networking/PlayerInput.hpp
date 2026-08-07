#pragma once
#include "Core/Platform.hpp"
#include "ShapeEngine/Math/Vector2.hpp"

namespace Shape {

struct PlayerInput {
    u32 frame = 0;
    Math::Vector2f targetPosition = {0.0f, 0.0f};
    u32 actionBits = 0;
};

} // namespace Shape
