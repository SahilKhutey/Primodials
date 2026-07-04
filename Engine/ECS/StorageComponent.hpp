#pragma once
#include "Core/Platform.hpp"

namespace Shape {

struct StorageComponent {
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
