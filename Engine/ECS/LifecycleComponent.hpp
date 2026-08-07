#pragma once
#include "ECS/ComponentTypes.hpp"

#include "Core/Platform.hpp"

namespace Shape {

struct LifecycleComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::LifecycleComponent);
    f32 age = 0.0f;
    f32 maxAge = 80.0f;
    f32 health = 100.0f;
    f32 energy = 50.0f;               // Energy value (0 to 100)
    
    f32 energyLossPerSecond = 1.0f;
    f32 reproductionThreshold = 85.0f; // split if energy goes above this
    
    u32 generation = 0;
    bool isDead = false;

    LifecycleComponent() = default;
};

} // namespace Shape
