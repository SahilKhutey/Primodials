// Shape/Simulation/Systems/NeuralControlSystem.cpp
#include "Shape/Simulation/Systems/NeuralControlSystem.h"
#include "ECS/TransformComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "ECS/LifecycleComponent.hpp"

namespace Shape::Simulation {

NeuralControlSystem::NeuralControlSystem(World* world)
    : m_world(world) {}

void NeuralControlSystem::update(const TickContext& ctx) {
    (void)ctx;
    if (!m_world) return;

    auto entities = m_world->Query<NeuralBrainComponent, TransformComponent, RigidBodyComponent>();

    for (Entity entity : entities) {
        auto& brain = m_world->GetComponent<NeuralBrainComponent>(entity);
        auto& transform = m_world->GetComponent<TransformComponent>(entity);
        auto& rb = m_world->GetComponent<RigidBodyComponent>(entity);

        // Assemble 4-channel sensory input vector:
        // [0] position X normalized, [1] position Y normalized, [2] velocity X, [3] velocity Y
        std::vector<f32> inputs = {
            transform.position.x * 0.01f,
            transform.position.y * 0.01f,
            rb.velocity.x * 0.1f,
            rb.velocity.y * 0.1f
        };

        brain.last_inputs = inputs;
        std::vector<f32> outputs = brain.network.Evaluate(inputs);
        brain.last_outputs = outputs;

        if (outputs.size() >= 2) {
            // Apply thrust forces based on neural outputs [-1, 1]
            f32 thrust_x = outputs[0] * 50.0f;
            f32 thrust_y = outputs[1] * 50.0f;
            rb.force.x += thrust_x;
            rb.force.y += thrust_y;
        }

        // Increment fitness score for staying active
        brain.fitness_score += static_cast<f32>(ctx.fixed_delta);
    }
}

} // namespace Shape::Simulation
