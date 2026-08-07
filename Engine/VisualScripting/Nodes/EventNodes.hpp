#pragma once

#include "VisualScripting/Core/Node.hpp"
#include "VisualScripting/Core/NodeGraph.hpp"

namespace Shape::VisualScripting {

class OnTickNode : public Node {
public:
    OnTickNode() : Node("OnTick", "On Tick", NodeCategory::Event) {
        AddExecOutput();
        AddOutput("Tick", DataType::Int);
        AddOutput("DeltaTime", DataType::Float);

        SetExecuteFn([](NodeGraph& graph, Node*, const std::vector<Value>&, std::vector<Value>& outputs) -> bool {
            outputs.resize(3);
            outputs[0] = Value(true);
            outputs[1] = Value(static_cast<i64>(graph.GetCurrentTick()));
            outputs[2] = Value(static_cast<f64>(graph.GetDeltaTime()));
            return true;
        });
    }
};

class OnEntitySpawnedNode : public Node {
public:
    OnEntitySpawnedNode() : Node("OnEntitySpawned", "On Entity Spawned", NodeCategory::Event) {
        AddExecOutput();
        AddOutput("Entity", DataType::Entity);
        AddOutput("Position", DataType::Vec2);

        SetExecuteFn([](NodeGraph&, Node*, const std::vector<Value>&, std::vector<Value>& outputs) -> bool {
            outputs.resize(3);
            outputs[0] = Value(true);
            outputs[1] = Value(static_cast<u64>(0));
            outputs[2] = Value(Math::Vector2f{ 0.0f, 0.0f });
            return true;
        });
    }
};

} // namespace Shape::VisualScripting
