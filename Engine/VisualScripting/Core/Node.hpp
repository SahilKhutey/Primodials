#pragma once

#include "VisualScripting/Core/Value.hpp"
#include "ShapeEngine/Math/Vector2.hpp"
#include <string>
#include <vector>
#include <functional>

namespace Shape { class World; }

namespace Shape::VisualScripting {

class NodeGraph;

enum class PinDirection { Input, Output };

struct Pin {
    std::string name;
    DataType type = DataType::Invalid;
    PinDirection direction = PinDirection::Input;
    Value defaultValue;
    bool hasDefault = false;
    u64 connectedNode = 0;
    std::string connectedPin;

    bool IsConnected() const { return connectedNode != 0; }
};

enum class NodeCategory { Event, Query, Action, Flow, Math, Utility };

inline const char* GetNodeCategoryName(NodeCategory c) {
    switch (c) {
        case NodeCategory::Event:   return "Events";
        case NodeCategory::Query:   return "Queries";
        case NodeCategory::Action:  return "Actions";
        case NodeCategory::Flow:    return "Flow";
        case NodeCategory::Math:    return "Math";
        case NodeCategory::Utility: return "Utility";
    }
    return "?";
}

class Node {
public:
    using ExecuteFn = std::function<bool(NodeGraph& graph, Node* self, const std::vector<Value>& inputs, std::vector<Value>& outputs)>;

    Node(std::string typeName, std::string displayName, NodeCategory category);
    virtual ~Node() = default;

    int AddInput(const std::string& name, DataType type, bool hasDefault = false, Value defaultValue = Value());
    int AddOutput(const std::string& name, DataType type);

    const std::vector<Pin>& GetInputs() const { return m_inputs; }
    const std::vector<Pin>& GetOutputs() const { return m_outputs; }
    const Pin* FindInput(const std::string& name) const;
    const Pin* FindOutput(const std::string& name) const;

    u64 GetId() const { return m_id; }
    void SetId(u64 id) { m_id = id; }
    const std::string& GetTypeName() const { return m_typeName; }
    const std::string& GetDisplayName() const { return m_displayName; }
    NodeCategory GetCategory() const { return m_category; }

    Math::Vector2f GetPosition() const { return m_position; }
    void SetPosition(const Math::Vector2f& pos) { m_position = pos; }

    void SetExecuteFn(ExecuteFn fn) { m_executeFn = std::move(fn); }
    bool Execute(NodeGraph& graph, const std::vector<Value>& inputs, std::vector<Value>& outputs) {
        if (m_executeFn) return m_executeFn(graph, this, inputs, outputs);
        return false;
    }

protected:
    void AddExecInput() { AddInput("▶", DataType::Exec); }
    void AddExecOutput() { AddOutput("▶", DataType::Exec); }

    std::string m_typeName;
    std::string m_displayName;
    NodeCategory m_category;
    u64 m_id = 0;
    Math::Vector2f m_position = { 0.0f, 0.0f };
    std::vector<Pin> m_inputs;
    std::vector<Pin> m_outputs;
    ExecuteFn m_executeFn;
};

} // namespace Shape::VisualScripting
