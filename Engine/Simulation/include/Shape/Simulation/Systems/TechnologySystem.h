// Shape/Simulation/Systems/TechnologySystem.h
#pragma once
#include "Simulation/ISimulationSystem.hpp"

namespace Shape::Simulation {
    class TechnologySystem : public ISimulationSystem {
    public:
        TechnologySystem() = default;
        virtual ~TechnologySystem() = default;

        void update(const TickContext& ctx) override;
        std::string_view name() const override { return "TechnologySystem"; }
        int priority() const override { return 110; }
    };
}
