// Shape/Procedural/WorldGenerator.cpp
#include "Shape/Procedural/WorldGenerator.h"
#include "Shape/Core/Logger.hpp"
#include "Shape/Random/DeterministicRng.hpp"

#include <algorithm>
#include <cmath>

namespace Shape::Procedural {

    WorldGenerator::WorldGenerator(const WorldGenConfig& cfg) : m_cfg(cfg) {
        m_elevation = SimplexNoise(cfg.seed);
        m_moisture  = SimplexNoise(cfg.seed ^ 0xDEADBEEFCAFEBABEull);
        m_temperature = SimplexNoise(cfg.seed ^ 0x123456789ABCDEF0ull);
    }

    void WorldGenerator::set_config(const WorldGenConfig& cfg) {
        m_cfg = cfg;
        m_elevation = SimplexNoise(cfg.seed);
        m_moisture  = SimplexNoise(cfg.seed ^ 0xDEADBEEFCAFEBABEull);
        m_temperature = SimplexNoise(cfg.seed ^ 0x123456789ABCDEF0ull);
    }

    BiomeInfo WorldGenerator::sample_at(Math::Vec2f world_pos) const {
        const float e = (m_elevation.fbm(world_pos.x * m_cfg.elevation_scale,
                                          world_pos.y * m_cfg.elevation_scale, m_cfg.octaves) + 1.0f) * 0.5f;
        const float m = (m_moisture.fbm(world_pos.x * m_cfg.moisture_scale,
                                         world_pos.y * m_cfg.moisture_scale, m_cfg.octaves) + 1.0f) * 0.5f;
        const float t = (m_temperature.fbm(world_pos.x * m_cfg.temperature_scale,
                                            world_pos.y * m_cfg.temperature_scale, 3) + 1.0f) * 0.5f;
        BiomeInfo info;
        info.elevation = e;
        info.moisture = m;
        info.temperature = t * 40.0f - 5.0f;
        info.type = classify(e, m, t);
        switch (info.type) {
            case Biome::Ocean:    info.color = {0.10f, 0.30f, 0.65f, 1}; info.vegetation_density = 0.0f; info.food_abundance = 0.3f; break;
            case Biome::Beach:    info.color = {0.95f, 0.90f, 0.65f, 1}; info.vegetation_density = 0.2f; info.food_abundance = 0.5f; break;
            case Biome::Plains:   info.color = {0.45f, 0.70f, 0.30f, 1}; info.vegetation_density = 0.5f; info.food_abundance = 1.0f; break;
            case Biome::Forest:   info.color = {0.20f, 0.50f, 0.20f, 1}; info.vegetation_density = 1.0f; info.food_abundance = 1.2f; break;
            case Biome::Desert:   info.color = {0.95f, 0.85f, 0.55f, 1}; info.vegetation_density = 0.05f; info.food_abundance = 0.2f; break;
            case Biome::Mountain: info.color = {0.55f, 0.50f, 0.45f, 1}; info.vegetation_density = 0.1f; info.food_abundance = 0.3f; break;
            case Biome::Tundra:   info.color = {0.85f, 0.90f, 0.95f, 1}; info.vegetation_density = 0.1f; info.food_abundance = 0.4f; break;
            case Biome::Swamp:    info.color = {0.35f, 0.45f, 0.30f, 1}; info.vegetation_density = 0.7f; info.food_abundance = 0.8f; break;
            case Biome::Jungle:   info.color = {0.10f, 0.55f, 0.20f, 1}; info.vegetation_density = 1.5f; info.food_abundance = 1.5f; break;
        }
        return info;
    }

    Biome WorldGenerator::classify(float elevation, float moisture, float temperature) const {
        if (elevation < m_cfg.sea_level) return Biome::Ocean;
        if (elevation < m_cfg.sea_level + 0.02f) return Biome::Beach;
        if (elevation > m_cfg.mountain_threshold) return Biome::Mountain;
        if (temperature < 0.2f) return Biome::Tundra;
        if (temperature > 0.7f && moisture < 0.3f) return Biome::Desert;
        if (temperature > 0.6f && moisture > 0.7f) return Biome::Jungle;
        if (moisture > 0.8f) return Biome::Swamp;
        if (moisture > 0.5f) return Biome::Forest;
        return Biome::Plains;
    }

    GeneratedWorld WorldGenerator::generate() const {
        GeneratedWorld world;
        world.bounds.min = {-m_cfg.width * 0.5f, -m_cfg.height * 0.5f};
        world.bounds.max = { m_cfg.width * 0.5f,  m_cfg.height * 0.5f};
        world.resolution = m_cfg.resolution;
        world.biomes.resize(m_cfg.resolution * m_cfg.resolution);

        for (int y = 0; y < m_cfg.resolution; ++y) {
            for (int x = 0; x < m_cfg.resolution; ++x) {
                const float wx = world.bounds.min.x + (static_cast<float>(x) / static_cast<float>(m_cfg.resolution)) * m_cfg.width;
                const float wy = world.bounds.min.y + (static_cast<float>(y) / static_cast<float>(m_cfg.resolution)) * m_cfg.height;
                world.biomes[y * m_cfg.resolution + x] = sample_at({wx, wy});
            }
        }
        return world;
    }

    const BiomeInfo& GeneratedWorld::at(Math::Vec2f world_pos) const {
        const float nx = (world_pos.x - bounds.min.x) / bounds.width();
        const float ny = (world_pos.y - bounds.min.y) / bounds.height();
        int cx = static_cast<int>(nx * resolution);
        int cy = static_cast<int>(ny * resolution);
        cx = Math::clamp(cx, 0, resolution - 1);
        cy = Math::clamp(cy, 0, resolution - 1);
        return biomes[cy * resolution + cx];
    }

    std::vector<WorldGenerator::ResourceSpot> WorldGenerator::place_food(int count) const {
        std::vector<ResourceSpot> spots;
        spots.reserve(count);
        Shape::Random::DeterministicRng rng(m_cfg.seed ^ 0xFOOD);
        
        const float w = m_cfg.width;
        const float h = m_cfg.height;
        for (int i = 0; i < count; ++i) {
            const float x = -w*0.5f + rng.NextFloat() * w;
            const float y = -h*0.5f + rng.NextFloat() * h;
            BiomeInfo info = sample_at({x, y});
            if (info.type != Biome::Ocean && info.type != Biome::Mountain) {
                if (rng.NextFloat() < info.food_abundance * m_cfg.food_density) {
                    spots.emplace_back(Math::Vec2f{x, y}, 100.0f + rng.NextFloat() * 100.0f);
                }
            }
        }
        return spots;
    }

} // namespace Shape::Procedural
