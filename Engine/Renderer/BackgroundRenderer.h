#pragma once

#include "Themes/Theme.h"
#include "Math/Vec4.hpp"
#include <cstdint>

namespace ShapeEngine::Renderer {

    class BackgroundRenderer {
    public:
        BackgroundRenderer();
        ~BackgroundRenderer();

        bool initialize();
        void shutdown();

        void render(uint32_t screenWidth, uint32_t screenHeight, 
                    const Themes::Theme& theme, float time);

        void setColorOverride(bool enable, 
                               const Shape::Math::Vec4f& topColor, 
                               const Shape::Math::Vec4f& bottomColor);
        void setVignetteIntensity(float intensity);

    private:
        bool m_initialized = false;
        bool m_colorOverride = false;
        Shape::Math::Vec4f m_overrideTopColor;
        Shape::Math::Vec4f m_overrideBottomColor;
        float m_vignetteIntensity = 1.0f;
    };

} // namespace ShapeEngine::Renderer
