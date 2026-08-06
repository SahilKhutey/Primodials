#include "VisualScripting/Core/NodeRegistry.hpp"
#include "VisualScripting/Nodes/EventNodes.hpp"
#include "VisualScripting/Nodes/ActionNodes.hpp"

namespace Shape::VisualScripting {

void NodeRegistry::RegisterNode(const std::string& typeName, const std::string& displayName, NodeCategory category, NodeFactory factory) {
    Registration reg;
    reg.info = { typeName, displayName, category };
    reg.factory = std::move(factory);
    m_registrations.push_back(std::move(reg));
}

std::unique_ptr<Node> NodeRegistry::CreateNode(const std::string& typeName) const {
    for (const auto& reg : m_registrations) {
        if (reg.info.typeName == typeName) return reg.factory();
    }
    return nullptr;
}

std::vector<NodeRegistry::NodeInfo> NodeRegistry::GetAllNodes() const {
    std::vector<NodeInfo> result;
    result.reserve(m_registrations.size());
    for (const auto& reg : m_registrations) {
        result.push_back(reg.info);
    }
    return result;
}

void NodeRegistry::RegisterDefaults() {
    RegisterNode("OnTick", "On Tick", NodeCategory::Event, []() { return std::make_unique<OnTickNode>(); });
    RegisterNode("OnEntitySpawned", "On Entity Spawned", NodeCategory::Event, []() { return std::make_unique<OnEntitySpawnedNode>(); });
    RegisterNode("SpawnEntity", "Spawn Entity", NodeCategory::Action, []() { return std::make_unique<SpawnEntityNode>(); });
    RegisterNode("DespawnEntity", "Despawn Entity", NodeCategory::Action, []() { return std::make_unique<DespawnEntityNode>(); });
}

} // namespace Shape::VisualScripting
