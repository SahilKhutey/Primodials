#pragma once

#include "VisualScripting/Core/Node.hpp"
#include "VisualScripting/Core/NodeGraph.hpp"
#include "ECS/TransformComponent.hpp"

namespace Shape::VisualScripting {

class SpawnEntityNode : public Node {
public:
    SpawnEntityNode() : Node("SpawnEntity", "Spawn Entity", NodeCategory::Action) {
        AddExecInput();
        AddInput("Position", DataType::Vec2, true, Value(Math::Vector2f{ 0.0f, 0.0f }));
        AddExecOutput();
        AddOutput("Entity", DataType::Entity);

        SetExecuteFn([](NodeGraph& graph, Node*, const std::vector<Value>& inputs, std::vector<Value>& outputs) -> bool {
            World* world = graph.GetWorld();
            if (!world) return false;

            Math::Vector2f pos = inputs.size() > 1 ? inputs[1].AsVec2() : Math::Vector2f{ 0.0f, 0.0f };
            Entity e = world->CreateEntity();
            world->AddComponent<TransformComponent>(e, TransformComponent{ .position = pos });

            outputs.resize(2);
            outputs[0] = Value(true);
            outputs[1] = Value(static_cast<u64>(e));
            return true;
        });
    }
};

class DespawnEntityNode : public Node {
public:
    DespawnEntityNode() : Node("DespawnEntity", "Despawn Entity", NodeCategory::Action) {
        AddExecInput();
        AddInput("Entity", DataType::Entity);
        AddExecOutput();

        SetExecuteFn([](NodeGraph& graph, Node*, const std::vector<Value>& inputs, std::vector<Value>& outputs) -> bool {
            World* world = graph.GetWorld();
            if (!world || inputs.size() < 2) return false;

            u64 entityId = inputs[1].AsEntity();
            if (world->IsEntityValid(static_cast<Entity>(entityId))) {
                world->DestroyEntity(static_cast<Entity>(entityId));
            }

            outputs.resize(1);
            outputs[0] = Value(true);
            return true;
        });
    }
};

} // namespace Shape::VisualScripting
