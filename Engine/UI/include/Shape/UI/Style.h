// Shape/UI/Style.h
// Theme & current-overlay singleton
#pragma once

#include "Shape/UI/IOverlayUI.h"
#include <memory>

namespace Shape::UI {

    struct Theme {
        Math::Color panel_bg{0.10f, 0.10f, 0.12f, 0.95f};
        Math::Color panel_border{0.30f, 0.30f, 0.35f, 1.0f};
        Math::Color text_color{0.95f, 0.95f, 0.95f, 1.0f};
        Math::Color text_secondary{0.65f, 0.65f, 0.70f, 1.0f};
        Math::Color accent{0.30f, 0.60f, 1.00f, 1.0f};
        Math::Color danger{1.00f, 0.30f, 0.30f, 1.0f};
        Math::Color success{0.30f, 0.95f, 0.30f, 1.0f};
        float panel_padding = 8.0f;
        float panel_rounding = 4.0f;
        float font_size = 14.0f;
    };

    class Style {
    public:
        static Style& instance();
        static IOverlayUI* current_overlay();
        static void set_current_overlay(IOverlayUI* ui);

        Theme& theme() { return m_theme; }
        const Theme& theme() const { return m_theme; }
        void set_theme(Theme t) { m_theme = t; }

        void apply_dark();
        void apply_light();

    private:
        Style() = default;
        Theme m_theme;
        IOverlayUI* m_current = nullptr;
    };

} // namespace Shape::UI
