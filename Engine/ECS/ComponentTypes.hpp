#pragma once
#include "Core/Platform.hpp"

namespace Shape {
    enum class ComponentTypeId : u32 {
        AIComponent = 1,
        CareerComponent = 2,
        CivilizationComponent = 3,
        GeneticsComponent = 4,
        GeometryComponent = 5,
        InfluenceComponent = 6,
        LifecycleComponent = 7,
        MindComponent = 8,
        ResourceComponent = 9,
        SentinelComponent = 10,
        StorageComponent = 11,
        TransformComponent = 12,
        ColliderComponent = 13,
        RigidBodyComponent = 14,
    };
} // namespace Shape
