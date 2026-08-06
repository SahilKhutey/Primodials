#include "UI/Panels/WorkshopBrowserPanel.hpp"

namespace Shape {

WorkshopBrowserPanel::WorkshopBrowserPanel(Steam::WorkshopManager* manager)
    : m_manager(manager) {}

void WorkshopBrowserPanel::Refresh() {}

void WorkshopBrowserPanel::Draw(World& world) {
    (void)world;
}

} // namespace Shape
