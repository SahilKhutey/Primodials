#include "VisualScripting/Core/Node.hpp"

namespace Shape::VisualScripting {

Node::Node(std::string typeName, std::string displayName, NodeCategory category)
    : m_typeName(std::move(typeName)), m_displayName(std::move(displayName)), m_category(category) {}

int Node::AddInput(const std::string& name, DataType type, bool hasDefault, Value defaultValue) {
    Pin pin;
    pin.name = name;
    pin.type = type;
    pin.direction = PinDirection::Input;
    pin.hasDefault = hasDefault;
    pin.defaultValue = std::move(defaultValue);
    m_inputs.push_back(pin);
    return static_cast<int>(m_inputs.size() - 1);
}

int Node::AddOutput(const std::string& name, DataType type) {
    Pin pin;
    pin.name = name;
    pin.type = type;
    pin.direction = PinDirection::Output;
    m_outputs.push_back(pin);
    return static_cast<int>(m_outputs.size() - 1);
}

const Pin* Node::FindInput(const std::string& name) const {
    for (const auto& pin : m_inputs) {
        if (pin.name == name) return &pin;
    }
    return nullptr;
}

const Pin* Node::FindOutput(const std::string& name) const {
    for (const auto& pin : m_outputs) {
        if (pin.name == name) return &pin;
    }
    return nullptr;
}

} // namespace Shape::VisualScripting
