#pragma once
#include "ECS/ComponentTypes.hpp"

#include "Core/Platform.hpp"

namespace Shape {

struct ResourceComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::ResourceComponent);
    f32 resourceAmount = 30.0f;     // Total energy food stores
    f32 maxResourceAmount = 30.0f;
    bool isDepleted = false;

    ResourceComponent() = default;
};

} // namespace Shape
