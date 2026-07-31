// Shape/Simulation/Systems/ProceduralWorldSystem.h
#pragma once
#include "Simulation/ISimulationSystem.hpp"
#include "Shape/Procedural/WorldGenerator.h"

namespace Shape::Simulation {
    class ProceduralWorldSystem : public ISimulationSystem {
    public:
        ProceduralWorldSystem() = default;
        virtual ~ProceduralWorldSystem() = default;

        void update(const TickContext& ctx) override;
        std::string_view name() const override { return "ProceduralWorldSystem"; }
        int priority() const override { return 113; }
    private:
        Shape::Procedural::WorldGenerator m_world_gen;
    };
}
