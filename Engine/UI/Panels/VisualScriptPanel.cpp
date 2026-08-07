#include "UI/Panels/VisualScriptPanel.hpp"

namespace Shape::UI {

VisualScriptPanel::VisualScriptPanel() {
    VisualScripting::NodeRegistry::Get().RegisterDefaults();
    m_graph = std::make_shared<VisualScripting::NodeGraph>();
    m_editor.SetGraph(m_graph);
}

void VisualScriptPanel::Draw(World& world) {
    m_executor.Initialize(&world);
    m_editor.Render();
}

} // namespace Shape::UI
