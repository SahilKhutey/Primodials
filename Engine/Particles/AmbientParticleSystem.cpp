#include "Particles/AmbientParticleSystem.hpp"
#include <cmath>

namespace Shape::Particles {

void AmbientParticleSystem::Initialize(usize maxParticles) {
    m_MaxParticles = maxParticles;
    m_Particles.clear();
    m_Particles.reserve(m_MaxParticles);
    m_ActiveCount = 0;
    m_SpawnTimer = 0.0f;
}

void AmbientParticleSystem::Update(f32 dt, f32 screenWidth, f32 screenHeight, const Math::Vec4f& primaryColor, f32 densityMultiplier) {
    usize targetCount = static_cast<usize>(static_cast<f32>(m_MaxParticles) * densityMultiplier);
    if (targetCount > m_MaxParticles) targetCount = m_MaxParticles;

    // Spawn new particles if below target count
    m_SpawnTimer += dt;
    if (m_Particles.size() < targetCount && m_SpawnTimer >= 0.02f) {
        m_SpawnTimer = 0.0f;
        SpawnParticle(screenWidth, screenHeight, primaryColor);
    }

    m_ActiveCount = 0;
    for (auto& p : m_Particles) {
        if (p.lifetime >= p.maxLifetime) continue;

        p.lifetime += dt;
        p.position.x += p.velocity.x * dt;
        p.position.y += p.velocity.y * dt;

        // Wrap around screen bounds
        if (p.position.x < 0.0f) p.position.x += screenWidth;
        if (p.position.x > screenWidth) p.position.x -= screenWidth;
        if (p.position.y < 0.0f) p.position.y += screenHeight;
        if (p.position.y > screenHeight) p.position.y -= screenHeight;

        // Pulse alpha
        f32 lifeRatio = p.lifetime / p.maxLifetime;
        f32 fade = (lifeRatio < 0.2f) ? (lifeRatio / 0.2f) : ((lifeRatio > 0.8f) ? (1.0f - lifeRatio) / 0.2f : 1.0f);
        p.color.w = fade * 0.7f;

        m_ActiveCount++;
    }
}

void AmbientParticleSystem::SpawnParticle(f32 screenWidth, f32 screenHeight, const Math::Vec4f& primaryColor) {
    AmbientParticle p;
    // Simple deterministic pseudo-random distribution based on particle index
    f32 seed = static_cast<f32>(m_Particles.size() * 17 + 31);
    f32 rx = std::fmod(seed * 0.1234f, 1.0f);
    f32 ry = std::fmod(seed * 0.5678f, 1.0f);
    f32 rvx = std::fmod(seed * 0.9101f, 1.0f) - 0.5f;
    f32 rvy = std::fmod(seed * 0.1121f, 1.0f) - 0.5f;

    p.position = Math::Vec2f(rx * screenWidth, ry * screenHeight);
    p.velocity = Math::Vec2f(rvx * 15.0f, rvy * 15.0f - 5.0f); // Gentle upward drift
    p.color = primaryColor;
    p.size = 1.5f + rvx * 2.0f;
    p.lifetime = 0.0f;
    p.maxLifetime = 4.0f + ry * 6.0f;
    p.type = ParticleType::BioluminescentMote;

    m_Particles.push_back(p);
}

} // namespace Shape::Particles
