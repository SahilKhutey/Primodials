#pragma once

#include "VisualScripting/Core/Node.hpp"
#include "ECS/World.hpp"
#include <memory>
#include <vector>

namespace Shape::VisualScripting {

struct Connection {
    u64 fromNode = 0;
    std::string fromPin;
    u64 toNode = 0;
    std::string toPin;

    bool operator==(const Connection& other) const {
        return fromNode == other.fromNode && fromPin == other.fromPin &&
               toNode == other.toNode && toPin == other.toPin;
    }
};

class NodeGraph {
public:
    NodeGraph() = default;
    ~NodeGraph() = default;

    u64 AddNode(std::unique_ptr<Node> node);
    bool RemoveNode(u64 nodeId);

    Node* GetNode(u64 nodeId) const;
    const std::vector<std::unique_ptr<Node>>& GetNodes() const { return m_nodes; }
    usize GetNodeCount() const { return m_nodes.size(); }

    bool AddConnection(u64 fromNode, const std::string& fromPin, u64 toNode, const std::string& toPin);
    bool RemoveConnection(u64 fromNode, const std::string& fromPin, u64 toNode, const std::string& toPin);

    const std::vector<Connection>& GetConnections() const { return m_connections; }

    World* GetWorld() const { return m_world; }
    void SetWorld(World* world) { m_world = world; }

    const std::string& GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

    u32 GetCurrentTick() const { return m_currentTick; }
    void SetCurrentTick(u32 tick) { m_currentTick = tick; }
    f32 GetDeltaTime() const { return m_deltaTime; }
    void SetDeltaTime(f32 dt) { m_deltaTime = dt; }

private:
    std::string m_name = "UntitledGraph";
    World* m_world = nullptr;
    u32 m_currentTick = 0;
    f32 m_deltaTime = 0.016f;

    std::vector<std::unique_ptr<Node>> m_nodes;
    std::vector<Connection> m_connections;
    u64 m_nextNodeId = 1;
};

} // namespace Shape::VisualScripting
