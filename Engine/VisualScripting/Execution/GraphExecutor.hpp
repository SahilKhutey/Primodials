#pragma once

#include "VisualScripting/Core/NodeGraph.hpp"
#include <unordered_map>
#include <vector>

namespace Shape::VisualScripting {

struct ExecutionStats {
    u64 executions = 0;
    u64 nodesExecuted = 0;
    f64 totalCPUTimeMs = 0.0;
    u32 cycleErrors = 0;
};

class GraphExecutor {
public:
    GraphExecutor() = default;
    ~GraphExecutor() = default;

    void Initialize(World* world) { m_world = world; }
    bool Execute(NodeGraph& graph);

    ExecutionStats GetStats() const { return m_stats; }
    void ResetStats() { m_stats = {}; }

private:
    bool TopologicallySort(NodeGraph& graph, std::vector<u64>& sorted);

    World* m_world = nullptr;
    ExecutionStats m_stats;
};

} // namespace Shape::VisualScripting
