// Shape/Simulation/Systems/QLearningSystem.cpp
#include "Shape/Simulation/Systems/QLearningSystem.h"
#include <algorithm>

namespace Shape::Simulation {

QLearningSystem::QLearningSystem(World* world)
    : m_world(world) {}

void QLearningSystem::update(const TickContext& ctx) {
    if (!m_world) return;

    auto entities = m_world->Query<QBrainComponent>();

    for (Entity entity : entities) {
        auto& qbrain = m_world->GetComponent<QBrainComponent>(entity);
        if (qbrain.q_table.empty()) {
            qbrain.q_table.resize(qbrain.num_states * qbrain.num_actions, 0.0f);
        }

        // Example state determination from entity index & tick modulo
        uint32_t current_state = static_cast<uint32_t>((entity + ctx.tick) % qbrain.num_states);
        uint32_t selected_action = select_action(qbrain, current_state, ctx.rng);

        // Calculate step reward (e.g. constant survival reward)
        float reward = 1.0f;
        uint32_t next_state = static_cast<uint32_t>((entity + ctx.tick + 1) % qbrain.num_states);

        update_q_value(qbrain, current_state, selected_action, reward, next_state);

        qbrain.last_state = current_state;
        qbrain.last_action = selected_action;
    }
}

uint32_t QLearningSystem::select_action(QBrainComponent& qbrain, uint32_t state, DeterministicRng& rng) {
    if (qbrain.q_table.empty()) {
        qbrain.q_table.resize(qbrain.num_states * qbrain.num_actions, 0.0f);
    }

    // Epsilon-greedy exploration
    if (rng.NextBool(qbrain.epsilon)) {
        return rng.NextU32() % qbrain.num_actions;
    }

    // Exploitation: argmax_a Q(s, a)
    uint32_t best_action = 0;
    float max_q = qbrain.get_q(state, 0);

    for (uint32_t a = 1; a < qbrain.num_actions; ++a) {
        float q = qbrain.get_q(state, a);
        if (q > max_q) {
            max_q = q;
            best_action = a;
        }
    }

    return best_action;
}

void QLearningSystem::update_q_value(QBrainComponent& qbrain, uint32_t state, uint32_t action, float reward, uint32_t next_state) {
    float max_next_q = qbrain.get_q(next_state, 0);
    for (uint32_t a = 1; a < qbrain.num_actions; ++a) {
        max_next_q = std::max(max_next_q, qbrain.get_q(next_state, a));
    }

    float current_q = qbrain.get_q(state, action);
    float target_q = reward + (qbrain.gamma * max_next_q);
    float updated_q = current_q + (qbrain.alpha * (target_q - current_q));

    qbrain.set_q(state, action, updated_q);
}

} // namespace Shape::Simulation
