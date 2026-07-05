// Shape/Simulation/Systems/TechnologySystem.h
#pragma once
#include "Shape/Simulation/Systems/ISimulationSystem.hpp"
namespace Shape::Simulation {
    class TechnologySystem : public ISimulationSystem {
    public:
        TechnologySystem() = default;
        virtual ~TechnologySystem() = default;
        
        void update(double dt) override;
        const char* name() const override { return "TechnologySystem"; }
        int priority() const override { return 110; }
    };
}
