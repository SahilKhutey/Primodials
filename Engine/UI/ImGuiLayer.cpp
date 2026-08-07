#include "UI/ImGuiLayer.hpp"

namespace Shape::UI {

Entity ImGuiLayer::s_selectedEntity = INVALID_ENTITY;
SimParams ImGuiLayer::s_simParams{};

void ImGuiLayer::AddPanel(std::unique_ptr<IPanel> panel) {
    if (panel) {
        m_panels.push_back(std::move(panel));
    }
}

void ImGuiLayer::DrawAllPanels(World& world) {
    for (auto& panel : m_panels) {
        if (panel) {
            panel->Draw(world);
        }
    }
}

} // namespace Shape::UI
