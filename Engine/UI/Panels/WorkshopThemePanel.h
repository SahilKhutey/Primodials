#pragma once

#include "Steam/Workshop/WorkshopThemeManager.h"
#include <memory>

namespace ShapeEngine {

    class WorkshopThemePanel {
    public:
        explicit WorkshopThemePanel(Steam::WorkshopThemeManager* manager);
        ~WorkshopThemePanel() = default;

        void render();
        void show() { m_visible = true; }
        void hide() { m_visible = false; }
        bool isVisible() const { return m_visible; }

    private:
        Steam::WorkshopThemeManager* m_manager;
        bool m_visible = false;
        [[maybe_unused]] char m_search[64] = "";
    };

} // namespace ShapeEngine
