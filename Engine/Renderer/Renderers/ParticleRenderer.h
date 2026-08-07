#pragma once

#include "Themes/Theme.h"
#include "Particles/AmbientParticleSystem.hpp"
#include "Math/Vec4.hpp"
#include <memory>
#include <unordered_map>
#include <string>

namespace ShapeEngine::Renderer {

    class ParticleRenderer {
    public:
        ParticleRenderer();
        ~ParticleRenderer();

        bool initialize();
        void shutdown();

        void render(Shape::Particles::AmbientParticleSystem* system, 
                    const Themes::Theme& theme,
                    float screenWidth, float screenHeight);

        void applyTheme(const Themes::Theme& theme);

    private:
        bool m_initialized = false;
        [[maybe_unused]] uint32_t m_maxParticles = 5000;

        struct TypeConfig {
            Shape::Math::Vec4f color;
            float     size;
            float     emissionRate;
            float     riseSpeed;
            float     driftSpeed;
            float     lifetime;
        };
        std::unordered_map<std::string, TypeConfig> m_typeConfigs;

        void buildTypeConfigs(const Themes::Theme& theme);
    };

} // namespace ShapeEngine::Renderer
