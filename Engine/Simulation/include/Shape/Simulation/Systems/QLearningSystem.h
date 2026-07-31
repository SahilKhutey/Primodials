// Shape/Simulation/Systems/QLearningSystem.h
#pragma once

#include "Simulation/ISimulationSystem.hpp"
#include "ECS/World.hpp"
#include "ECS/ComponentTypes.hpp"
#include <string_view>
#include <vector>

namespace Shape::Simulation {

struct QBrainComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::AIComponent);
    uint32_t num_states = 16;
    uint32_t num_actions = 4;
    std::vector<f32> q_table; // Size = num_states * num_actions

    float alpha = 0.1f;    // Learning rate
    float gamma = 0.9f;    // Discount factor
    float epsilon = 0.2f;  // Exploration rate

    uint32_t last_state = 0;
    uint32_t last_action = 0;

    float get_q(uint32_t state, uint32_t action) const {
        if (state >= num_states || action >= num_actions || q_table.empty()) return 0.0f;
        usize idx = static_cast<usize>(state) * num_actions + action;
        if (idx >= q_table.size()) return 0.0f;
        return q_table[idx];
    }

    void set_q(uint32_t state, uint32_t action, float value) {
        if (state >= num_states || action >= num_actions) return;
        if (q_table.size() < static_cast<usize>(num_states) * num_actions) {
            q_table.resize(static_cast<usize>(num_states) * num_actions, 0.0f);
        }
        usize idx = static_cast<usize>(state) * num_actions + action;
        q_table[idx] = value;
    }
};

class QLearningSystem : public ISimulationSystem {
public:
    QLearningSystem() = default;
    explicit QLearningSystem(World* world);
    virtual ~QLearningSystem() = default;

    void set_world(World* world) noexcept { m_world = world; }
    void update(const TickContext& ctx) override;
    std::string_view name() const override { return "QLearningSystem"; }
    int priority() const override { return 26; }

    uint32_t select_action(QBrainComponent& qbrain, uint32_t state, DeterministicRng& rng);
    void update_q_value(QBrainComponent& qbrain, uint32_t state, uint32_t action, float reward, uint32_t next_state);

private:
    World* m_world = nullptr;
};

} // namespace Shape::Simulation
