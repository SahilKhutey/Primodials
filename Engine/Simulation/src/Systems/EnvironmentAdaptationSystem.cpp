// Shape/Simulation/Systems/EnvironmentAdaptationSystem.cpp
#include "Shape/Simulation/Systems/EnvironmentAdaptationSystem.h"
#include <algorithm>
#include <cmath>

namespace Shape::Simulation {

EnvironmentAdaptationSystem::EnvironmentAdaptationSystem(World* world, SpeciesRegistry* registry, WeatherSystem* weather, BiomeSystem* biome)
    : m_world(world), m_registry(registry), m_weather_system(weather), m_biome_system(biome) {}

float EnvironmentAdaptationSystem::calculate_biome_suitability(const Species& species, BiomeType biome) {
    float score = 0.5f; // Neutral baseline

    switch (biome) {
        case BiomeType::LushForest:
            // High moisture, high fertility favors creatures with high oxygen/water requirements
            if (species.biology.waterRequirement <= 1.2f) score += 0.2f;
            if (species.biology.temperatureToleranceMax >= 20.0f) score += 0.2f;
            break;
        case BiomeType::AridDesert:
            // Low moisture tolerance, high heat resistance needed
            if (species.biology.waterRequirement <= 0.5f) score += 0.3f;
            if (species.biology.temperatureToleranceMax >= 35.0f) score += 0.2f;
            else score -= 0.3f;
            break;
        case BiomeType::Tundra:
            // Cold tolerance needed
            if (species.biology.temperatureToleranceMin <= -5.0f) score += 0.4f;
            else score -= 0.4f;
            break;
        case BiomeType::VolcanicCrater:
            // Extreme heat and radiation resistance required
            if (species.biology.temperatureToleranceMax >= 45.0f && species.biology.radiationResistance >= 0.4f) {
                score += 0.4f;
            } else {
                score -= 0.4f;
            }
            break;
        case BiomeType::DeepOcean:
            // High water requirement and pressure tolerance
            if (species.biology.waterRequirement >= 1.0f && species.biology.pressureTolerance >= 1.2f) {
                score += 0.4f;
            } else {
                score -= 0.3f;
            }
            break;
    }

    return std::clamp(score, 0.0f, 1.0f);
}

void EnvironmentAdaptationSystem::update(const TickContext& ctx) {
    if (!m_world || !m_registry || !m_weather_system || !m_biome_system) {
        return;
    }

    const auto& env_state = m_weather_system->state();
    float temp_celsius = env_state.temperature_celsius;
    float global_drain_mult = env_state.metabolic_drain_multiplier;

    std::vector<Entity> entities = m_world->Query<TransformComponent, LifecycleComponent, SpeciesIdComponent>();

    for (Entity entity : entities) {
        TransformComponent& trans = m_world->GetComponent<TransformComponent>(entity);
        LifecycleComponent& life = m_world->GetComponent<LifecycleComponent>(entity);
        SpeciesIdComponent& spec_id = m_world->GetComponent<SpeciesIdComponent>(entity);

        if (!m_registry->IsValid(spec_id.id)) continue;
        const Species& species = m_registry->Get(spec_id.id);

        // Get or attach AdaptationStatusComponent
        if (!m_world->HasComponent<AdaptationStatusComponent>(entity)) {
            m_world->AddComponent(entity, AdaptationStatusComponent{});
        }
        auto& status = m_world->GetComponent<AdaptationStatusComponent>(entity);

        // 1. Thermal Stress Calculation
        status.thermal_stress = 0.0f;
        if (temp_celsius < species.biology.temperatureToleranceMin) {
            status.thermal_stress = (species.biology.temperatureToleranceMin - temp_celsius) * 0.05f;
        } else if (temp_celsius > species.biology.temperatureToleranceMax) {
            status.thermal_stress = (temp_celsius - species.biology.temperatureToleranceMax) * 0.05f;
        }

        // 2. Biome Properties & Suitability
        auto biome_props = m_biome_system->get_properties_at(trans.position);
        float suitability = calculate_biome_suitability(species, biome_props.type);
        status.is_in_preferred_biome = (suitability >= 0.6f);

        // 3. Moisture Satisfaction
        if (biome_props.moisture < species.biology.waterRequirement * 0.3f) {
            status.moisture_satisfaction = std::max(0.1f, status.moisture_satisfaction - 0.01f);
        } else {
            status.moisture_satisfaction = std::min(1.0f, status.moisture_satisfaction + 0.02f);
        }

        // 4. Effective Metabolic Rate Scaling
        float base_metabolism = species.biology.metabolismRate;
        float thermal_penalty = 1.0f + status.thermal_stress;
        float dehydration_penalty = (1.5f - status.moisture_satisfaction * 0.5f);
        status.effective_metabolic_rate = base_metabolism * global_drain_mult * thermal_penalty * dehydration_penalty;

        // Apply energy drain to lifecycle component
        float dt_seconds = static_cast<float>(ctx.fixed_delta);
        float energy_drain = status.effective_metabolic_rate * dt_seconds;
        life.energy = std::max(0.0f, life.energy - energy_drain);

        // If energy depleted or severe thermal stress, take health damage
        if (life.energy <= 0.0f || status.thermal_stress > 1.0f) {
            float damage = (life.energy <= 0.0f ? 2.0f : 0.0f) + status.thermal_stress * 1.5f;
            life.health = std::max(0.0f, life.health - damage * dt_seconds);
        }

        // 5. Movement Speed Adaptation (scale linear damping inverse to suitability)
        status.movement_speed_multiplier = std::clamp(suitability / std::max(0.2f, biome_props.movement_friction * species.physics.frictionCoefficient), 0.2f, 2.0f);

        if (m_world->HasComponent<RigidBodyComponent>(entity)) {
            auto& rb = m_world->GetComponent<RigidBodyComponent>(entity);
            rb.linearDamping = std::clamp(0.01f / status.movement_speed_multiplier, 0.005f, 0.5f);
        }
    }
}

} // namespace Shape::Simulation
