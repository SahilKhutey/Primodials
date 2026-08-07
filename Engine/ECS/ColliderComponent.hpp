#pragma once
#include "ShapeEngine/Math/Vector2.hpp"
#include "Core/Platform.hpp"

namespace Shape {

enum class ColliderType : u8 {
    None,
    Circle,
    AABB
};

struct ColliderComponent {
    ColliderType type = ColliderType::None;
    
    // For Circle
    f32 radius = 0.0f;
    
    // For AABB
    Math::Vector2f extents = Math::Vector2f::Zero(); // Half-widths
    
    // Settings
    bool isStatic = false;
    bool isTrigger = false;
    
    // Create Helpers
    static ColliderComponent CreateCircle(f32 r, bool staticObj = false, bool trigger = false) {
        ColliderComponent c;
        c.type = ColliderType::Circle;
        c.radius = r;
        c.isStatic = staticObj;
        c.isTrigger = trigger;
        return c;
    }
    
    static ColliderComponent CreateAABB(Math::Vector2f halfExtents, bool staticObj = false, bool trigger = false) {
        ColliderComponent c;
        c.type = ColliderType::AABB;
        c.extents = halfExtents;
        c.isStatic = staticObj;
        c.isTrigger = trigger;
        return c;
    }
};

} // namespace Shape
