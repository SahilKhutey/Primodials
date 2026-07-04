#pragma once
#include "ECS/ComponentTypes.hpp"

#include "Core/Platform.hpp"

namespace Shape {

enum class SentinelType : u8 {
    Military,
    Economic,
    Science,
    Construction
};

struct SentinelComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::SentinelComponent);
    SentinelType type = SentinelType::Economic;
    f32 commandRadius = 60.0f;
    f32 syncTimer = 0.0f;
    f32 syncInterval = 0.5f; // Sync memories twice per second

    SentinelComponent() = default;
    
    explicit SentinelComponent(SentinelType t, f32 radius = 60.0f)
        : type(t)
        , commandRadius(radius) {}
};

} // namespace Shape
