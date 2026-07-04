#pragma once
#include "ECS/ComponentTypes.hpp"
#include "Core/Platform.hpp"
#include <vector>

namespace Shape {

enum class FactionAlignment {
    Neutral,
    Allied,
    Hostile
};

struct CivilizationComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::CivilizationComponent);
    FactionAlignment alignment = FactionAlignment::Neutral;
    u32 factionId = 0;
    
    std::vector<u32> unlockedTechnologies;

    bool HasTechnology(u32 techId) const {
        for (u32 id : unlockedTechnologies) {
            if (id == techId) return true;
        }
        return false;
    }
};

} // namespace Shape
