#pragma once
#include "ECS/ComponentTypes.hpp"

#include "ShapeEngine/Math/Vector2.hpp"

namespace Shape {

struct RigidBodyComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::RigidBodyComponent);
    Math::Vector2f velocity = Math::Vector2f::Zero();
    Math::Vector2f force = Math::Vector2f::Zero();

    f32 angularVelocity = 0.0f;
    f32 torque = 0.0f;

    f32 mass = 1.0f;
    f32 inverseMass = 1.0f;       // 0.0f indicates static/unmovable object
    
    f32 inertia = 1.0f;
    f32 inverseInertia = 1.0f;    // 0.0f indicates static/unmovable rotation

    // Drag / Damping
    f32 linearDamping = 0.01f;
    f32 angularDamping = 0.01f;

    RigidBodyComponent() = default;

    void SetMass(f32 m) {
        mass = m;
        if (m > 0.0f) {
            inverseMass = 1.0f / m;
        } else {
            inverseMass = 0.0f; // Static
        }
    }

    void SetInertia(f32 I) {
        inertia = I;
        if (I > 0.0f) {
            inverseInertia = 1.0f / I;
        } else {
            inverseInertia = 0.0f; // Static
        }
    }
};

} // namespace Shape
