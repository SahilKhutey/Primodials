// Shape/Simulation/Systems/ProceduralWorldSystem.h
#pragma once
#include "Shape/Simulation/Systems/ISimulationSystem.hpp"
#include "Shape/Procedural/WorldGenerator.h"
namespace Shape::Simulation {
    class ProceduralWorldSystem : public ISimulationSystem {
    public:
        ProceduralWorldSystem() = default;
        virtual ~ProceduralWorldSystem() = default;
        void update(double dt) override;
        const char* name() const override { return "ProceduralWorldSystem"; }
        int priority() const override { return 113; }
    private:
        Shape::Procedural::WorldGenerator m_world_gen;
    };
}
