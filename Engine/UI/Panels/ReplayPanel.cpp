#include "UI/Panels/ReplayPanel.hpp"
#include "UI/ImGuiLayer.hpp"

namespace Shape::UI {

ReplayPanel::ReplayPanel(Simulation::ReplaySystem* replaySystem)
    : m_replaySystem(replaySystem) {}

void ReplayPanel::Draw(World& world) {
    (void)world;
    if (!m_replaySystem) return;
}

} // namespace Shape::UI
