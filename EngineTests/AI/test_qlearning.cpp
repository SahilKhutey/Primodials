#include <catch2/catch_test_macros.hpp>
#include "Shape/Simulation/Systems/QLearningSystem.h"
#include "ECS/World.hpp"

TEST_CASE("QLearningSystem - Q-Table & TD Updates", "[AI][QLearning]") {
    Shape::World world;
    Shape::Simulation::QLearningSystem q_sys(&world);
    Shape::Simulation::DeterministicRng rng(888);

    SECTION("System metadata") {
        REQUIRE(q_sys.name() == "QLearningSystem");
        REQUIRE(q_sys.priority() == 26);
    }

    SECTION("Q-Table Initialization & Bellman Update") {
        Shape::Simulation::QBrainComponent qbrain;
        qbrain.num_states = 4;
        qbrain.num_actions = 2;
        qbrain.alpha = 0.5f;
        qbrain.gamma = 0.9f;

        // Q-value before update
        REQUIRE(qbrain.get_q(0, 1) == 0.0f);

        // Perform TD-update: state 0, action 1, reward 10.0, next_state 1
        q_sys.update_q_value(qbrain, 0, 1, 10.0f, 1);

        // Target Q = 10.0 + 0.9 * 0.0 = 10.0
        // Updated Q = 0.0 + 0.5 * (10.0 - 0.0) = 5.0
        REQUIRE(qbrain.get_q(0, 1) == 5.0f);
    }

    SECTION("System Update Cycle") {
        Shape::Entity entity = world.CreateEntity();
        Shape::Simulation::QBrainComponent qbrain;
        qbrain.num_states = 4;
        qbrain.num_actions = 2;
        world.AddComponent(entity, qbrain);

        Shape::TickContext ctx{1, 1.0 / 60.0, 0.016, rng};
        q_sys.update(ctx);

        auto& updated_q = world.GetComponent<Shape::Simulation::QBrainComponent>(entity);
        REQUIRE_FALSE(updated_q.q_table.empty());
    }
}
