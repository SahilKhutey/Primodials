#pragma once
#include "ECS/ComponentTypes.hpp"
#include "Core/Platform.hpp"

namespace Shape {

struct StorageComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::StorageComponent);
    f32 biomass = 0.0f;
    f32 minerals = 0.0f;
    f32 sciencePoints = 0.0f;

    f32 maxCapacity = 1000.0f;

    bool AddBiomass(f32 amount) {
        if (biomass + minerals + amount > maxCapacity) return false;
        biomass += amount;
        return true;
    }

    bool AddMinerals(f32 amount) {
        if (biomass + minerals + amount > maxCapacity) return false;
        minerals += amount;
        return true;
    }
};

} // namespace Shape
