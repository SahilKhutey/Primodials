// Shape/Simulation/Systems/DiplomacySystem.h
#pragma once
#include "Simulation/ISimulationSystem.hpp"

namespace Shape::Simulation {
    class DiplomacySystem : public ISimulationSystem {
    public:
        DiplomacySystem() = default;
        virtual ~DiplomacySystem() = default;

        void update(const TickContext& ctx) override;
        std::string_view name() const override { return "DiplomacySystem"; }
        int priority() const override { return 111; }
    };
}
