#pragma once

#include "UI/IPanel.hpp"
#include "VisualScripting/Editor/NodeGraphEditor.hpp"
#include "VisualScripting/Execution/GraphExecutor.hpp"

namespace Shape::UI {

class VisualScriptPanel final : public IPanel {
public:
    VisualScriptPanel();
    ~VisualScriptPanel() override = default;

    void Draw(World& world) override;
    const char* GetName() const override { return "Visual Scripting"; }

private:
    VisualScripting::NodeGraphEditor m_editor;
    VisualScripting::GraphExecutor m_executor;
    std::shared_ptr<VisualScripting::NodeGraph> m_graph;
};

} // namespace Shape::UI
