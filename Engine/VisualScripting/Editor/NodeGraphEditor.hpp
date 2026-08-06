#pragma once

#include "VisualScripting/Core/NodeGraph.hpp"
#include "VisualScripting/Core/NodeRegistry.hpp"
#include <memory>

namespace Shape::VisualScripting {

class NodeGraphEditor {
public:
    NodeGraphEditor() = default;
    ~NodeGraphEditor() = default;

    void SetGraph(std::shared_ptr<NodeGraph> graph) { m_graph = graph; }
    std::shared_ptr<NodeGraph> GetGraph() const { return m_graph; }

    void Render();

private:
    std::shared_ptr<NodeGraph> m_graph;
};

} // namespace Shape::VisualScripting
