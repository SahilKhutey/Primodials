// Shape/Simulation/Systems/BiomeSystem.cpp
#include "Shape/Simulation/Systems/BiomeSystem.h"
#include <cmath>

namespace Shape::Simulation {

BiomeSystem::BiomeSystem() = default;

void BiomeSystem::update(const TickContext& ctx) {
    (void)ctx;
    // Biome system maintains spatial biome properties per tick
}

BiomeType BiomeSystem::get_biome_at(Math::Vector2f position) const {
    // Deterministic spatial hash/grid formula for biomes
    float val = std::sin(position.x * m_scale) * std::cos(position.y * m_scale);
    float noise = std::sin(position.x * 0.02f + position.y * 0.02f) * 0.5f;
    float combined = val + noise;

    if (combined < -0.5f) return BiomeType::DeepOcean;
    if (combined < -0.1f) return BiomeType::Tundra;
    if (combined <  0.3f) return BiomeType::LushForest;
    if (combined <  0.7f) return BiomeType::AridDesert;
    return BiomeType::VolcanicCrater;
}

BiomeProperties BiomeSystem::get_properties_at(Math::Vector2f position) const {
    return get_biome_properties(get_biome_at(position));
}

BiomeProperties BiomeSystem::get_biome_properties(BiomeType type) {
    BiomeProperties props;
    props.type = type;

    switch (type) {
        case BiomeType::LushForest:
            props.name = "LushForest";
            props.temperature_offset = 0.0f;
            props.moisture = 0.8f;
            props.fertility_multiplier = 1.5f;
            props.movement_friction = 1.0f;
            break;
        case BiomeType::AridDesert:
            props.name = "AridDesert";
            props.temperature_offset = 12.0f;
            props.moisture = 0.1f;
            props.fertility_multiplier = 0.2f;
            props.movement_friction = 1.2f; // Sand friction
            break;
        case BiomeType::Tundra:
            props.name = "Tundra";
            props.temperature_offset = -15.0f;
            props.moisture = 0.4f;
            props.fertility_multiplier = 0.4f;
            props.movement_friction = 1.3f; // Snow friction
            break;
        case BiomeType::VolcanicCrater:
            props.name = "VolcanicCrater";
            props.temperature_offset = 25.0f;
            props.moisture = 0.05f;
            props.fertility_multiplier = 0.1f;
            props.movement_friction = 1.5f;
            break;
        case BiomeType::DeepOcean:
            props.name = "DeepOcean";
            props.temperature_offset = -5.0f;
            props.moisture = 1.0f;
            props.fertility_multiplier = 0.8f;
            props.movement_friction = 2.0f; // Water drag
            break;
    }
    return props;
}

} // namespace Shape::Simulation
