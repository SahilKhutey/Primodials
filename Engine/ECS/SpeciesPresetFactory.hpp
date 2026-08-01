// Shape/ECS/SpeciesPresetFactory.hpp
#pragma once

#include "ECS/SpeciesSchema.hpp"
#include "ECS/SpeciesRegistry.hpp"

namespace Shape {

/// Factory that creates default Polygonal Primordials species archetypes.
class SpeciesPresetFactory {
public:
    /// Create Triangles: Fast Apex Predators (high combat, high speed, low health)
    static Species CreateApexTriangle() {
        Species s{};
        s.geometry.vertexCount = 3;
        s.geometry.circumradius = 6.0f;
        s.geometry.primaryColor = Color{230, 50, 50, 255}; // Red

        s.biology.maxHealth = 80.0f;
        s.biology.maxEnergy = 100.0f;
        s.biology.metabolismRate = 1.2f;
        s.biology.temperatureToleranceMin = -5.0f;
        s.biology.temperatureToleranceMax = 45.0f;
        s.biology.waterRequirement = 0.6f;

        s.military.combatSkillBase = 8.0f;
        s.military.meleeCombat = 0.9f;

        s.physics.maxSpeed = 80.0f;
        s.physics.acceleration = 25.0f;
        s.physics.frictionCoefficient = 0.3f;

        s.RecomputeDerived();
        return s;
    }

    /// Create Quadrilaterals: Tough Builders & Crafters (high health, high defense, building efficiency)
    static Species CreateBuilderQuad() {
        Species s{};
        s.geometry.vertexCount = 4;
        s.geometry.circumradius = 10.0f;
        s.geometry.primaryColor = Color{50, 120, 220, 255}; // Blue

        s.biology.maxHealth = 150.0f;
        s.biology.maxEnergy = 120.0f;
        s.biology.metabolismRate = 0.8f;
        s.biology.temperatureToleranceMin = -15.0f;
        s.biology.temperatureToleranceMax = 38.0f;
        s.biology.waterRequirement = 0.8f;

        s.construction.buildingSpeed = 1.8f;
        s.construction.buildingQuality = 0.9f;
        s.construction.miningSkill = 0.8f;

        s.physics.maxSpeed = 45.0f;
        s.physics.acceleration = 10.0f;
        s.physics.frictionCoefficient = 0.6f;

        s.RecomputeDerived();
        return s;
    }

    /// Create Pentagons: Adaptable Nomads (high environmental tolerance, balanced stats)
    static Species CreateNomadPentagon() {
        Species s{};
        s.geometry.vertexCount = 5;
        s.geometry.circumradius = 12.0f;
        s.geometry.primaryColor = Color{50, 200, 100, 255}; // Green

        s.biology.maxHealth = 110.0f;
        s.biology.maxEnergy = 140.0f;
        s.biology.metabolismRate = 0.7f;
        s.biology.temperatureToleranceMin = -25.0f;
        s.biology.temperatureToleranceMax = 50.0f;
        s.biology.waterRequirement = 0.4f; // Highly drought-resistant

        s.psychology.curiosity = 0.9f;
        s.psychology.riskTolerance = 0.7f;

        s.physics.maxSpeed = 60.0f;
        s.physics.acceleration = 15.0f;
        s.physics.frictionCoefficient = 0.4f;

        s.RecomputeDerived();
        return s;
    }

    /// Create Hexagons: High-Tech Hive Engineers (high intelligence, fast research, strong diplomacy)
    static Species CreateHiveHexagon() {
        Species s{};
        s.geometry.vertexCount = 6;
        s.geometry.circumradius = 14.0f;
        s.geometry.primaryColor = Color{220, 180, 40, 255}; // Amber/Gold

        s.biology.maxHealth = 100.0f;
        s.biology.maxEnergy = 90.0f;
        s.biology.metabolismRate = 0.9f;

        s.intelligence.rawIq = 130.0f;
        s.intelligence.learningRate = 1.8f;
        s.technology.researchSpeed = 2.0f;
        s.society.cooperationSkill = 0.95f;

        s.physics.maxSpeed = 50.0f;
        s.physics.acceleration = 12.0f;

        s.RecomputeDerived();
        return s;
    }

    /// Register all default Primordials species into a SpeciesRegistry instance.
    static void RegisterDefaults(SpeciesRegistry& registry) {
        registry.Register(CreateApexTriangle());
        registry.Register(CreateBuilderQuad());
        registry.Register(CreateNomadPentagon());
        registry.Register(CreateHiveHexagon());
    }
};

} // namespace Shape
