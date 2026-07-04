#pragma once

#include "Core/Platform.hpp"

namespace Shape {

enum class ColliderType : u8 {
    Circle,
    Polygon
};

struct ColliderComponent {
    ColliderType type = ColliderType::Circle;

    // Physical Material properties
    f32 restitution = 0.5f;   // Restitution/bounciness (0.0 = clay, 1.0 = superball)
    f32 friction = 0.3f;      // Friction coefficient

    // Circle shape properties
    f32 radius = 1.0f;

    // Polygon properties are retrieved from the GeometryComponent to avoid vertex duplication
    ColliderComponent() = default;
};

} // namespace Shape
