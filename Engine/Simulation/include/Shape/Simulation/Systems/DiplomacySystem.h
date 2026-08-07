// Shape/Simulation/Systems/DiplomacySystem.h
#pragma once
#include "Shape/Simulation/Systems/ISimulationSystem.hpp"
namespace Shape::Simulation {
    class DiplomacySystem : public ISimulationSystem {
    public:
        DiplomacySystem() = default;
        virtual ~DiplomacySystem() = default;
        void update(double dt) override;
        const char* name() const override { return "DiplomacySystem"; }
        int priority() const override { return 111; }
    };
}
