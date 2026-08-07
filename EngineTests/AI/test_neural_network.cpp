#include <catch2/catch_test_macros.hpp>
#include "AI/NeuralNetwork.hpp"
#include "Shape/Simulation/Systems/NeuralControlSystem.h"
#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"

TEST_CASE("NeuralNetwork - Forward Pass, Mutation & Crossover", "[AI][NeuralNetwork]") {
    SECTION("Forward Evaluation") {
        Shape::AI::NeuralNetwork nn({4, 8, 2}, Shape::AI::ActivationFunction::Tanh);
        REQUIRE(nn.layer_sizes().size() == 3);

        std::vector<f32> inputs = {0.5f, -0.2f, 1.0f, 0.0f};
        std::vector<f32> outputs = nn.Evaluate(inputs);

        REQUIRE(outputs.size() == 2);
        REQUIRE(outputs[0] >= -1.0f);
        REQUIRE(outputs[0] <= 1.0f);
        REQUIRE(outputs[1] >= -1.0f);
        REQUIRE(outputs[1] <= 1.0f);
    }

    SECTION("Deterministic Mutation") {
        Shape::AI::NeuralNetwork nn1({4, 8, 2});
        Shape::AI::NeuralNetwork nn2 = nn1;

        Shape::Simulation::DeterministicRng rng(42);
        nn1.Mutate(rng, 1.0f, 0.5f); // Mutate all weights

        std::vector<f32> inputs = {1.0f, 1.0f, 1.0f, 1.0f};
        std::vector<f32> out1 = nn1.Evaluate(inputs);
        std::vector<f32> out2 = nn2.Evaluate(inputs);

        REQUIRE(out1 != out2); // Mutation altered network output
    }

    SECTION("Genetic Crossover") {
        Shape::AI::NeuralNetwork parent_a({4, 8, 2});
        Shape::AI::NeuralNetwork parent_b({4, 8, 2});

        Shape::Simulation::DeterministicRng rng(100);
        parent_a.Mutate(rng, 0.8f, 1.0f);

        Shape::AI::NeuralNetwork child = Shape::AI::NeuralNetwork::Crossover(parent_a, parent_b, rng);
        REQUIRE(child.layer_sizes() == parent_a.layer_sizes());
    }
}

TEST_CASE("NeuralControlSystem - Execution & Actuation", "[AI][NeuralControlSystem]") {
    Shape::World world;
    Shape::Simulation::NeuralControlSystem sys(&world);
    Shape::Simulation::DeterministicRng rng(777);

    REQUIRE(sys.name() == "NeuralControlSystem");
    REQUIRE(sys.priority() == 25);

    Shape::Entity entity = world.CreateEntity();
    Shape::TransformComponent transform;
    world.AddComponent(entity, transform);

    Shape::RigidBodyComponent rb;
    world.AddComponent(entity, rb);

    Shape::Simulation::NeuralBrainComponent brain;
    brain.network.initialize({4, 8, 2});
    world.AddComponent(entity, brain);

    Shape::TickContext ctx{1, 1.0 / 60.0, 0.016, rng};
    sys.update(ctx);

    auto& updated_brain = world.GetComponent<Shape::Simulation::NeuralBrainComponent>(entity);
    REQUIRE(updated_brain.last_inputs.size() == 4);
    REQUIRE(updated_brain.last_outputs.size() == 2);
    REQUIRE(updated_brain.fitness_score > 0.0f);
}
