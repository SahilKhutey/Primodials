// Shape/Procedural/WorldGenerator.h
// Generate a complete world with biomes and resources
#pragma once

#include "Shape/Procedural/SimplexNoise.h"
#include "Shape/Simulation/SimulationState.hpp" // Has WorldBounds
#include "Shape/Math/Vec2.hpp"
#include "Shape/Math/Color.hpp"

#include <cstdint>
#include <vector>
#include <memory>

namespace Shape::Procedural {

    enum class Biome : uint8_t {
        Ocean, Beach, Plains, Forest, Desert, Mountain, Tundra, Swamp, Jungle
    };

    struct BiomeInfo {
        Biome type = Biome::Plains;
        float temperature = 20.0f;     // Celsius
        float moisture = 0.5f;         // 0-1
        float elevation = 0.0f;        // 0-1
        Math::Color color{0.5f, 0.5f, 0.5f, 1};
        float vegetation_density = 0.5f;
        float food_abundance = 1.0f;
    };

    struct WorldGenConfig {
        uint64_t seed = 42;
        float width = 1200.0f;
        float height = 800.0f;
        int resolution = 256;  // Grid resolution for biome map
        float elevation_scale = 0.005f;
        float moisture_scale = 0.008f;
        float temperature_scale = 0.003f;
        int   octaves = 5;
        float sea_level = 0.45f;
        float mountain_threshold = 0.75f;
        float food_density = 1.0f;
    };

    struct GeneratedWorld {
        Shape::Simulation::WorldBounds bounds;
        std::vector<BiomeInfo> biomes;  // Indexed by [y*resolution + x]
        int resolution = 0;
        Math::Vec2i grid_size() const { return {resolution, resolution}; }
        const BiomeInfo& at(Math::Vec2f world_pos) const;
    };

    class WorldGenerator {
    public:
        explicit WorldGenerator(const WorldGenConfig& cfg = {});

        void set_config(const WorldGenConfig& cfg);
        const WorldGenConfig& config() const { return m_cfg; }

        GeneratedWorld generate() const;

        // Single-cell queries
        BiomeInfo sample_at(Math::Vec2f world_pos) const;
        Biome classify(float elevation, float moisture, float temperature) const;

        // Resource placement
        struct ResourceSpot {
            ResourceSpot(Math::Vec2f p, float a) : position(p), amount(a) {}
            Math::Vec2f position;
            float amount;
        };
        std::vector<ResourceSpot> place_food(int count) const;

    private:
        WorldGenConfig m_cfg;
        mutable SimplexNoise m_elevation;
        mutable SimplexNoise m_moisture;
        mutable SimplexNoise m_temperature;
    };

} // namespace Shape::Procedural
