#pragma once

#include "Core/Platform.hpp"
#include "Math/Vec2.hpp"
#include "Math/Vec4.hpp"
#include <vector>

namespace Shape::Particles {

enum class ParticleType {
    BioluminescentMote,
    WaterBubble,
    AtmosphericDust
};

struct AmbientParticle {
    Math::Vec2f position{0.0f, 0.0f};
    Math::Vec2f velocity{0.0f, 0.0f};
    Math::Vec4f color{1.0f, 1.0f, 1.0f, 1.0f};
    f32 size = 2.0f;
    f32 lifetime = 0.0f;
    f32 maxLifetime = 5.0f;
    ParticleType type = ParticleType::BioluminescentMote;
};

class AmbientParticleSystem {
public:
    AmbientParticleSystem() = default;
    ~AmbientParticleSystem() = default;

    void Initialize(usize maxParticles = 2000);
    void Update(f32 dt, f32 screenWidth, f32 screenHeight, const Math::Vec4f& primaryColor, f32 densityMultiplier = 1.0f);

    const std::vector<AmbientParticle>& GetParticles() const { return m_Particles; }
    usize GetActiveCount() const { return m_ActiveCount; }

private:
    void SpawnParticle(f32 screenWidth, f32 screenHeight, const Math::Vec4f& primaryColor);

    std::vector<AmbientParticle> m_Particles;
    usize m_ActiveCount = 0;
    usize m_MaxParticles = 2000;
    f32 m_SpawnTimer = 0.0f;
};

} // namespace Shape::Particles
