#pragma once

#include "Theme.h"
#include "UI/SettingsState.h"
#include "Renderer/BackgroundRenderer.h"
#include "Renderer/Renderers/ParticleRenderer.h"
#include "Particles/AmbientParticleSystem.hpp"

namespace ShapeEngine::Themes {

    class ThemeApplier {
    public:
        ThemeApplier(
            UI::SettingsState* settings,
            Renderer::BackgroundRenderer* bgRenderer,
            Renderer::ParticleRenderer* particleRenderer,
            Shape::Particles::AmbientParticleSystem* particleSystem);

        void initialize();
        void shutdown();

        void applyAll();

        void applyTheme(const Theme& theme);

        Theme getEffectiveTheme() const;

    private:
        void onSettingsChanged(const std::string& key);

        UI::SettingsState* m_settings;
        Renderer::BackgroundRenderer* m_bgRenderer;
        Renderer::ParticleRenderer* m_particleRenderer;
        [[maybe_unused]] Shape::Particles::AmbientParticleSystem* m_particleSystem;

        Theme m_lastTheme;
    };

} // namespace ShapeEngine::Themes
