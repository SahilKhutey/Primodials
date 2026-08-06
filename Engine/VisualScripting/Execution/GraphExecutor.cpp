#include "VisualScripting/Execution/GraphExecutor.hpp"
#include <queue>
#include <unordered_set>

namespace Shape::VisualScripting {

bool GraphExecutor::Execute(NodeGraph& graph) {
    if (m_world) graph.SetWorld(m_world);
    m_stats.executions++;

    std::vector<u64> sortedOrder;
    if (!TopologicallySort(graph, sortedOrder)) {
        m_stats.cycleErrors++;
        return false;
    }

    std::unordered_map<u64, std::unordered_map<std::string, Value>> nodeOutputs;

    for (u64 nodeId : sortedOrder) {
        Node* node = graph.GetNode(nodeId);
        if (!node) continue;

        std::vector<Value> inputs;
        inputs.reserve(node->GetInputs().size());

        for (const auto& pin : node->GetInputs()) {
            if (pin.type == DataType::Exec) {
                inputs.push_back(Value(true));
                continue;
            }

            bool connectedValFound = false;
            for (const auto& conn : graph.GetConnections()) {
                if (conn.toNode == nodeId && conn.toPin == pin.name) {
                    auto outIt = nodeOutputs.find(conn.fromNode);
                    if (outIt != nodeOutputs.end()) {
                        auto valIt = outIt->second.find(conn.fromPin);
                        if (valIt != outIt->second.end()) {
                            inputs.push_back(valIt->second);
                            connectedValFound = true;
                            break;
                        }
                    }
                }
            }

            if (!connectedValFound) {
                inputs.push_back(pin.hasDefault ? pin.defaultValue : Value());
            }
        }

        std::vector<Value> outputs;
        if (node->Execute(graph, inputs, outputs)) {
            m_stats.nodesExecuted++;
            for (usize i = 0; i < node->GetOutputs().size() && i < outputs.size(); ++i) {
                nodeOutputs[nodeId][node->GetOutputs()[i].name] = outputs[i];
            }
        }
    }

    return true;
}

bool GraphExecutor::TopologicallySort(NodeGraph& graph, std::vector<u64>& sorted) {
    std::unordered_map<u64, u32> inDegree;
    std::unordered_map<u64, std::vector<u64>> adjList;

    for (const auto& node : graph.GetNodes()) {
        inDegree[node->GetId()] = 0;
    }

    for (const auto& conn : graph.GetConnections()) {
        inDegree[conn.toNode]++;
        adjList[conn.fromNode].push_back(conn.toNode);
    }

    std::queue<u64> q;
    for (const auto& [id, deg] : inDegree) {
        if (deg == 0) q.push(id);
    }

    sorted.clear();
    while (!q.empty()) {
        u64 curr = q.front();
        q.pop();
        sorted.push_back(curr);

        for (u64 neighbor : adjList[curr]) {
            if (--inDegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    return sorted.size() == graph.GetNodeCount();
}

} // namespace Shape::VisualScripting
