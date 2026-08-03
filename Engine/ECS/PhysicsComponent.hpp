#pragma once
#include "ShapeEngine/Math/Vector2.hpp"
#include "Core/Platform.hpp"

namespace Shape {

struct PhysicsComponent {
    Math::Vector2f velocity = Math::Vector2f::Zero();
    Math::Vector2f acceleration = Math::Vector2f::Zero();
    Math::Vector2f forceAccumulator = Math::Vector2f::Zero();
    
    f32 mass = 1.0f;
    f32 inverseMass = 1.0f;
    f32 friction = 0.5f;
    f32 restitution = 0.5f; // Bounciness

    void AddForce(const Math::Vector2f& force) {
        forceAccumulator = forceAccumulator + force;
    }
};

} // namespace Shape
