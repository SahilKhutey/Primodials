#include "UI/Panels/NetworkPanel.hpp"
#include "UI/ImGuiLayer.hpp"

namespace Shape::UI {

NetworkPanel::NetworkPanel(Network::NetworkManager* networkManager)
    : m_networkManager(networkManager) {}

void NetworkPanel::Draw(World& world) {
    (void)world;
    if (!m_networkManager) return;
}

} // namespace Shape::UI
