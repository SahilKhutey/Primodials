#include "ThemeApplier.h"
#include "ThemeManager.h"
#include "Core/Logger.hpp"

namespace ShapeEngine::Themes {

    ThemeApplier::ThemeApplier(
        UI::SettingsState* settings,
        Renderer::BackgroundRenderer* bgRenderer,
        Renderer::ParticleRenderer* particleRenderer,
        Shape::Particles::AmbientParticleSystem* particleSystem)
        : m_settings(settings), m_bgRenderer(bgRenderer),
          m_particleRenderer(particleRenderer), m_particleSystem(particleSystem) {}

    void ThemeApplier::initialize() {
        m_settings->setOnChange([this](const std::string& key) {
            onSettingsChanged(key);
        });

        applyAll();
        SHAPE_LOG_INFO("ThemeApplier initialized");
    }

    void ThemeApplier::shutdown() {}

    void ThemeApplier::applyAll() {
        ThemeManager tm;
        tm.initialize("Content/Themes");
        auto theme = tm.loadTheme(m_settings->currentThemeId);
        if (!theme) {
            SHAPE_LOG_ERROR("Could not load theme: {}", m_settings->currentThemeId);
            return;
        }
        applyTheme(*theme);
    }

    void ThemeApplier::applyTheme(const Theme& theme) {
        m_lastTheme = theme;

        if (m_bgRenderer) {
            if (m_settings->useCustomColors) {
                m_bgRenderer->setColorOverride(true,
                    Shape::Math::Vec4f(m_settings->customBackgroundTop[0],
                                      m_settings->customBackgroundTop[1],
                                      m_settings->customBackgroundTop[2],
                                      m_settings->customBackgroundTop[3]),
                    Shape::Math::Vec4f(m_settings->customBackgroundBottom[0],
                                      m_settings->customBackgroundBottom[1],
                                      m_settings->customBackgroundBottom[2],
                                      m_settings->customBackgroundBottom[3]));
            } else {
                m_bgRenderer->setColorOverride(false, Shape::Math::Vec4f(0), Shape::Math::Vec4f(0));
            }
            m_bgRenderer->setVignetteIntensity(
                theme.vignetteIntensity * m_settings->vignetteIntensityOverride);
        }

        if (m_particleRenderer) {
            m_particleRenderer->applyTheme(theme);
        }

        SHAPE_LOG_INFO("Applied theme: {}", theme.displayName);
    }

    void ThemeApplier::onSettingsChanged(const std::string& key) {
        if (key == "theme" || key == "profile") {
            applyAll();
        } else if (key == "glow" || key == "vignette") {
            if (m_bgRenderer) {
                Theme effective = getEffectiveTheme();
                m_bgRenderer->setVignetteIntensity(
                    effective.vignetteIntensity * m_settings->vignetteIntensityOverride);
            }
        }
        m_settings->save("Content/_user/settings.json");
    }

    Theme ThemeApplier::getEffectiveTheme() const {
        Theme t = m_lastTheme;
        t.glowIntensity *= m_settings->glowIntensityOverride;
        t.particleDensity *= m_settings->particleDensityOverride;
        t.motionSpeed *= m_settings->motionSpeedOverride;
        t.populationDensity *= m_settings->populationDensityOverride;
        t.vignetteIntensity *= m_settings->vignetteIntensityOverride;
        return t;
    }

} // namespace ShapeEngine::Themes
