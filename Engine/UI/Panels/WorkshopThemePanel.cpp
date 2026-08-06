#include "WorkshopThemePanel.h"
#include "Core/Logger.hpp"

namespace ShapeEngine {

    WorkshopThemePanel::WorkshopThemePanel(Steam::WorkshopThemeManager* manager)
        : m_manager(manager) {
        if (m_manager) m_manager->requestThemeList();
    }

    void WorkshopThemePanel::render() {
        if (!m_visible || !m_manager) return;
    }

} // namespace ShapeEngine
