#pragma once

#include "VisualScripting/Core/Node.hpp"
#include <memory>
#include <vector>
#include <functional>

namespace Shape::VisualScripting {

class NodeRegistry {
public:
    using NodeFactory = std::function<std::unique_ptr<Node>()>;

    static NodeRegistry& Get() {
        static NodeRegistry instance;
        return instance;
    }

    void RegisterNode(const std::string& typeName, const std::string& displayName, NodeCategory category, NodeFactory factory);
    std::unique_ptr<Node> CreateNode(const std::string& typeName) const;

    struct NodeInfo {
        std::string typeName;
        std::string displayName;
        NodeCategory category;
    };
    std::vector<NodeInfo> GetAllNodes() const;

    void RegisterDefaults();

private:
    NodeRegistry() = default;

    struct Registration {
        NodeInfo info;
        NodeFactory factory;
    };
    std::vector<Registration> m_registrations;
};

} // namespace Shape::VisualScripting
