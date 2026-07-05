// Shape/Simulation/Systems/GroupBehaviorSystem.h
#pragma once
#include "Shape/Simulation/Systems/ISimulationSystem.hpp"
namespace Shape::Simulation {
    class GroupBehaviorSystem : public ISimulationSystem {
    public:
        GroupBehaviorSystem() = default;
        virtual ~GroupBehaviorSystem() = default;
        void update(double dt) override;
        const char* name() const override { return "GroupBehaviorSystem"; }
        int priority() const override { return 112; }
    };
}
