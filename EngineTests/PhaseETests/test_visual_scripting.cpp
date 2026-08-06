#include <catch2/catch_test_macros.hpp>
#include "VisualScripting/Core/DataType.hpp"
#include "VisualScripting/Core/Value.hpp"
#include "VisualScripting/Core/Node.hpp"
#include "VisualScripting/Core/NodeGraph.hpp"
#include "VisualScripting/Core/NodeRegistry.hpp"
#include "VisualScripting/Execution/GraphExecutor.hpp"
#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"

TEST_CASE("Visual Scripting Subsystem - Core Types, Node Graph, Registry, and Executor", "[VisualScripting]") {
    SECTION("DataType compatibility and Value variant getters") {
        REQUIRE(Shape::VisualScripting::AreTypesCompatible(Shape::VisualScripting::DataType::Int, Shape::VisualScripting::DataType::Float));
        REQUIRE(Shape::VisualScripting::AreTypesCompatible(Shape::VisualScripting::DataType::Entity, Shape::VisualScripting::DataType::Any));
        REQUIRE_FALSE(Shape::VisualScripting::AreTypesCompatible(Shape::VisualScripting::DataType::String, Shape::VisualScripting::DataType::Vec2));

        Shape::VisualScripting::Value valInt(42);
        REQUIRE(valInt.GetType() == Shape::VisualScripting::DataType::Int);
        REQUIRE(valInt.AsInt() == 42);

        Shape::VisualScripting::Value valVec(Shape::Math::Vector2f{ 10.0f, 20.0f });
        REQUIRE(valVec.GetType() == Shape::VisualScripting::DataType::Vec2);
        REQUIRE(valVec.AsVec2().x == 10.0f);
        REQUIRE(valVec.AsVec2().y == 20.0f);
    }

    SECTION("NodeRegistry default node creation") {
        auto& registry = Shape::VisualScripting::NodeRegistry::Get();
        registry.RegisterDefaults();

        auto nodeTick = registry.CreateNode("OnTick");
        REQUIRE(nodeTick != nullptr);
        REQUIRE(nodeTick->GetCategory() == Shape::VisualScripting::NodeCategory::Event);
        REQUIRE(nodeTick->GetOutputs().size() == 3);

        auto nodeSpawn = registry.CreateNode("SpawnEntity");
        REQUIRE(nodeSpawn != nullptr);
        REQUIRE(nodeSpawn->GetCategory() == Shape::VisualScripting::NodeCategory::Action);
        REQUIRE(nodeSpawn->GetInputs().size() == 2);
    }

    SECTION("NodeGraph connection management and validation") {
        Shape::VisualScripting::NodeGraph graph;

        auto n1 = std::make_unique<Shape::VisualScripting::Node>("ConstVal", "Const Val", Shape::VisualScripting::NodeCategory::Utility);
        n1->AddOutput("Value", Shape::VisualScripting::DataType::Vec2);
        u64 id1 = graph.AddNode(std::move(n1));

        auto n2 = Shape::VisualScripting::NodeRegistry::Get().CreateNode("SpawnEntity");
        u64 id2 = graph.AddNode(std::move(n2));

        REQUIRE(graph.AddConnection(id1, "Value", id2, "Position"));
        REQUIRE(graph.GetConnections().size() == 1);
        REQUIRE_FALSE(graph.AddConnection(id1, "Value", id2, "Position")); // Duplicate connection prevented
    }

    SECTION("GraphExecutor topological execution and entity spawning action") {
        Shape::World world;
        Shape::VisualScripting::NodeGraph graph;
        graph.SetWorld(&world);

        auto tickNode = Shape::VisualScripting::NodeRegistry::Get().CreateNode("OnTick");
        u64 tickId = graph.AddNode(std::move(tickNode));

        auto spawnNode = Shape::VisualScripting::NodeRegistry::Get().CreateNode("SpawnEntity");
        u64 spawnId = graph.AddNode(std::move(spawnNode));

        REQUIRE(graph.AddConnection(tickId, "▶", spawnId, "▶"));

        Shape::VisualScripting::GraphExecutor executor;
        executor.Initialize(&world);

        REQUIRE(executor.Execute(graph));
        REQUIRE(executor.GetStats().executions == 1);
        REQUIRE(executor.GetStats().nodesExecuted == 2);

        auto entities = world.Query<Shape::TransformComponent>();
        REQUIRE(entities.size() == 1);
    }
}
