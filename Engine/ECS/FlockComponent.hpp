// Engine/ECS/FlockComponent.hpp
#pragma once

#include "ECS/ComponentTypes.hpp"
#include "Core/Platform.hpp"

namespace Shape {

struct FlockComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::FlockComponent);

    u32 flock_id = 1;
    f32 perception_radius = 50.0f;
    f32 separation_weight = 1.5f;
    f32 alignment_weight = 1.0f;
    f32 cohesion_weight = 1.0f;
    f32 max_speed = 100.0f;
    f32 max_force = 40.0f;
};

} // namespace Shape
