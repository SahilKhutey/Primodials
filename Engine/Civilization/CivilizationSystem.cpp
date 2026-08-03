#include "CivilizationSystem.hpp"
#include "ECS/StorageComponent.hpp"
#include "ECS/CivilizationComponent.hpp"
#include "ECS/TransformComponent.hpp"

namespace Shape {

void CivilizationSystem::Update(World& world, f32 dt) {
    ProcessLogistics(world, dt);
    ProcessTechnology(world, dt);
}

void CivilizationSystem::ProcessLogistics(World& world, f32 dt) {
    // Basic fixed-rate transfer logic between allied storage depots
    auto entities = world.Query<TransformComponent, StorageComponent, CivilizationComponent>();
    
    // N^2 naive distance check for basic prototyping
    f32 transferRate = 10.0f * dt; // transfer 10 units per second
    f32 tradeRadiusSq = 200.0f * 200.0f;

    for (usize i = 0; i < entities.size(); ++i) {
        Entity e1 = entities[i];
        auto& pos1 = world.GetComponent<TransformComponent>(e1).position;
        auto& storage1 = world.GetComponent<StorageComponent>(e1);
        auto& civ1 = world.GetComponent<CivilizationComponent>(e1);

        if (civ1.alignment != FactionAlignment::Allied) continue;

        for (usize j = i + 1; j < entities.size(); ++j) {
            Entity e2 = entities[j];
            auto& pos2 = world.GetComponent<TransformComponent>(e2).position;
            auto& storage2 = world.GetComponent<StorageComponent>(e2);
            auto& civ2 = world.GetComponent<CivilizationComponent>(e2);

            if (civ2.alignment != FactionAlignment::Allied) continue;
            if (civ1.factionId != civ2.factionId) continue;

            f32 distSq = (pos1 - pos2).LengthSquared();
            if (distSq < tradeRadiusSq) {
                // Balance minerals if difference is significant
                if (storage1.minerals > storage2.minerals + transferRate) {
                    storage1.minerals -= transferRate;
                    storage2.minerals += transferRate;
                } else if (storage2.minerals > storage1.minerals + transferRate) {
                    storage2.minerals -= transferRate;
                    storage1.minerals += transferRate;
                }
            }
        }
    }
}

void CivilizationSystem::ProcessTechnology(World& world, f32 dt) {
    (void)dt;
    auto entities = world.Query<StorageComponent, CivilizationComponent>();
    
    const f32 TECH_COST = 100.0f; // Example cost to unlock tech ID 1

    for (Entity entity : entities) {
        auto& storage = world.GetComponent<StorageComponent>(entity);
        auto& civ = world.GetComponent<CivilizationComponent>(entity);

        // Simple tech unlock logic: if enough science, unlock Tech 1
        if (!civ.HasTechnology(1) && storage.sciencePoints >= TECH_COST) {
            storage.sciencePoints -= TECH_COST;
            civ.unlockedTechnologies.push_back(1);
        }
    }
}

} // namespace Shape
