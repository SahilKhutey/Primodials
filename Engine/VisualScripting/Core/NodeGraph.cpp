#include "VisualScripting/Core/NodeGraph.hpp"

namespace Shape::VisualScripting {

u64 NodeGraph::AddNode(std::unique_ptr<Node> node) {
    if (!node) return 0;
    u64 id = m_nextNodeId++;
    node->SetId(id);
    m_nodes.push_back(std::move(node));
    return id;
}

bool NodeGraph::RemoveNode(u64 nodeId) {
    auto it = std::remove_if(m_nodes.begin(), m_nodes.end(),
        [nodeId](const std::unique_ptr<Node>& n) { return n->GetId() == nodeId; });
    if (it == m_nodes.end()) return false;
    m_nodes.erase(it, m_nodes.end());

    m_connections.erase(std::remove_if(m_connections.begin(), m_connections.end(),
        [nodeId](const Connection& c) { return c.fromNode == nodeId || c.toNode == nodeId; }),
        m_connections.end());

    return true;
}

Node* NodeGraph::GetNode(u64 nodeId) const {
    for (const auto& node : m_nodes) {
        if (node->GetId() == nodeId) return node.get();
    }
    return nullptr;
}

bool NodeGraph::AddConnection(u64 fromNode, const std::string& fromPin, u64 toNode, const std::string& toPin) {
    Node* src = GetNode(fromNode);
    Node* dst = GetNode(toNode);
    if (!src || !dst || fromNode == toNode) return false;

    const Pin* outPin = src->FindOutput(fromPin);
    const Pin* inPin = dst->FindInput(toPin);
    if (!outPin || !inPin) return false;

    if (!AreTypesCompatible(outPin->type, inPin->type)) return false;

    Connection conn{ fromNode, fromPin, toNode, toPin };
    if (std::find(m_connections.begin(), m_connections.end(), conn) != m_connections.end()) {
        return false;
    }

    m_connections.push_back(conn);
    return true;
}

bool NodeGraph::RemoveConnection(u64 fromNode, const std::string& fromPin, u64 toNode, const std::string& toPin) {
    Connection conn{ fromNode, fromPin, toNode, toPin };
    auto it = std::find(m_connections.begin(), m_connections.end(), conn);
    if (it == m_connections.end()) return false;
    m_connections.erase(it);
    return true;
}

} // namespace Shape::VisualScripting
