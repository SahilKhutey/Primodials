// Shape/Simulation/Systems/BiomeSystem.h
#pragma once

#include "Simulation/ISimulationSystem.hpp"
#include "Math/Include/ShapeEngine/Math/Vector2.hpp"
#include <string_view>
#include <vector>

namespace Shape::Simulation {

enum class BiomeType {
    LushForest,
    AridDesert,
    Tundra,
    VolcanicCrater,
    DeepOcean
};

struct BiomeProperties {
    BiomeType type = BiomeType::LushForest;
    std::string_view name = "LushForest";
    float temperature_offset = 0.0f;     // °C offset relative to global weather
    float moisture = 0.7f;                // 0.0 to 1.0
    float fertility_multiplier = 1.2f;    // Resource/food growth multiplier
    float movement_friction = 1.0f;       // Movement speed multiplier
};

class BiomeSystem : public ISimulationSystem {
public:
    BiomeSystem();
    virtual ~BiomeSystem() = default;

    void update(const TickContext& ctx) override;
    std::string_view name() const override { return "BiomeSystem"; }
    int priority() const override { return -35; } // Pre-Sim environment setup after Weather

    BiomeType get_biome_at(Math::Vector2f position) const;
    BiomeProperties get_properties_at(Math::Vector2f position) const;

    static BiomeProperties get_biome_properties(BiomeType type);

private:
    float m_scale = 0.005f;
};

} // namespace Shape::Simulation
