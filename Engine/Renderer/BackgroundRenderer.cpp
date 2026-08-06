#include "BackgroundRenderer.h"
#include "Core/Logger.hpp"

namespace ShapeEngine::Renderer {

    BackgroundRenderer::BackgroundRenderer() = default;
    BackgroundRenderer::~BackgroundRenderer() { shutdown(); }

    bool BackgroundRenderer::initialize() {
        m_initialized = true;
        SHAPE_LOG_INFO("BackgroundRenderer initialized");
        return true;
    }

    void BackgroundRenderer::shutdown() {
        m_initialized = false;
    }

    void BackgroundRenderer::render(uint32_t screenWidth, uint32_t screenHeight, 
                                     const Themes::Theme& theme, float time) {
        (void)screenWidth;
        (void)screenHeight;
        (void)theme;
        (void)time;
        if (!m_initialized) return;
    }

    void BackgroundRenderer::setColorOverride(bool enable,
                                                const Shape::Math::Vec4f& topColor, 
                                                const Shape::Math::Vec4f& bottomColor) {
        m_colorOverride = enable;
        m_overrideTopColor = topColor;
        m_overrideBottomColor = bottomColor;
    }

    void BackgroundRenderer::setVignetteIntensity(float intensity) {
        m_vignetteIntensity = intensity;
    }

} // namespace ShapeEngine::Renderer
