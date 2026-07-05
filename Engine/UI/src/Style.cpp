// Shape/UI/Style.cpp
#include "Shape/UI/Style.h"

namespace Shape::UI {

    Style& Style::instance() {
        static Style s;
        return s;
    }

    IOverlayUI* Style::current_overlay() { return instance().m_current; }
    void Style::set_current_overlay(IOverlayUI* ui) { instance().m_current = ui; }

    void Style::apply_dark() {
        Theme t;
        t.panel_bg = {0.10f, 0.10f, 0.12f, 0.95f};
        t.panel_border = {0.30f, 0.30f, 0.35f, 1.0f};
        t.text_color = {0.95f, 0.95f, 0.95f, 1.0f};
        t.accent = {0.30f, 0.60f, 1.00f, 1.0f};
        m_theme = t;
    }

    void Style::apply_light() {
        Theme t;
        t.panel_bg = {0.95f, 0.95f, 0.97f, 0.95f};
        t.panel_border = {0.50f, 0.50f, 0.55f, 1.0f};
        t.text_color = {0.10f, 0.10f, 0.12f, 1.0f};
        t.accent = {0.20f, 0.50f, 0.90f, 1.0f};
        m_theme = t;
    }

} // namespace Shape::UI
