#include "ParticleRenderer.h"
#include "Core/Logger.hpp"

namespace ShapeEngine::Renderer {

    ParticleRenderer::ParticleRenderer() = default;
    ParticleRenderer::~ParticleRenderer() { shutdown(); }

    bool ParticleRenderer::initialize() {
        m_initialized = true;
        SHAPE_LOG_INFO("ParticleRenderer initialized");
        return true;
    }

    void ParticleRenderer::shutdown() {
        m_initialized = false;
    }

    void ParticleRenderer::applyTheme(const Themes::Theme& theme) {
        buildTypeConfigs(theme);
    }

    void ParticleRenderer::buildTypeConfigs(const Themes::Theme& theme) {
        Shape::Math::Vec4f moteColor = !theme.speciesPalette.empty() 
            ? theme.speciesPalette[0] : theme.accentColor;

        m_typeConfigs["bubble"] = { theme.glowColor, 4.0f, 20.0f, 15.0f, 0.0f, 8.0f };
        m_typeConfigs["mote"]   = { moteColor, 2.5f, 40.0f, 0.0f, 8.0f, 12.0f };
        m_typeConfigs["spark"]  = { theme.glowColor, 6.0f, 5.0f, 0.0f, 0.0f, 1.5f };
        m_typeConfigs["snow"]   = { Shape::Math::Vec4f(0.95f, 0.95f, 1.0f, 0.7f), 3.0f, 30.0f, -25.0f, 5.0f, 15.0f };
        m_typeConfigs["pollen"] = { Shape::Math::Vec4f(1.0f, 0.9f, 0.5f, 0.6f), 2.0f, 25.0f, 2.0f, 6.0f, 18.0f };
    }

    void ParticleRenderer::render(Shape::Particles::AmbientParticleSystem* system, 
                                    const Themes::Theme& theme,
                                    float screenWidth, float screenHeight) {
        (void)system;
        (void)theme;
        (void)screenWidth;
        (void)screenHeight;
        if (!m_initialized) return;
    }

} // namespace ShapeEngine::Renderer
