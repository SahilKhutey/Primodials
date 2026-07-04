#pragma once
#include "ECS/ComponentTypes.hpp"

#include "Core/Platform.hpp"

namespace Shape {

enum class InfluenceType : u8 {
    Fear,
    Leadership,
    Knowledge
};

struct InfluenceComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::InfluenceComponent);
    InfluenceType type = InfluenceType::Fear;
    f32 strength = 10.0f;
    f32 radius = 50.0f;

    InfluenceComponent() = default;
    
    InfluenceComponent(InfluenceType t, f32 s, f32 r)
        : type(t)
        , strength(s)
        , radius(r) {}
};

} // namespace Shape
