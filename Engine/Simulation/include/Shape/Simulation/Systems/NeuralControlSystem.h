// Shape/Simulation/Systems/NeuralControlSystem.h
#pragma once

#include "Simulation/ISimulationSystem.hpp"
#include "AI/NeuralNetwork.hpp"
#include "ECS/World.hpp"
#include "ECS/ComponentTypes.hpp"
#include <string_view>
#include <vector>

namespace Shape::Simulation {

struct NeuralBrainComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::MindComponent); // Reuse MindComponent slot or dedicated
    AI::NeuralNetwork network;
    std::vector<f32> last_inputs;
    std::vector<f32> last_outputs;
    f32 fitness_score = 0.0f;
};

class NeuralControlSystem : public ISimulationSystem {
public:
    NeuralControlSystem() = default;
    explicit NeuralControlSystem(World* world);
    virtual ~NeuralControlSystem() = default;

    void set_world(World* world) noexcept { m_world = world; }
    void update(const TickContext& ctx) override;
    std::string_view name() const override { return "NeuralControlSystem"; }
    int priority() const override { return 25; } // AI / Decision priority

private:
    World* m_world = nullptr;
};

} // namespace Shape::Simulation
