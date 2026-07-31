// Shape/Simulation/Systems/BuildingSystem.cpp
#include "Shape/Simulation/Systems/BuildingSystem.h"
#include "ECS/TransformComponent.hpp"
#include "ECS/StorageComponent.hpp"

namespace Shape::Simulation {

BuildingSystem::BuildingSystem(World* world)
    : m_world(world) {}

void BuildingSystem::update(const TickContext& ctx) {
    if (!m_world) return;
    float dt = static_cast<float>(ctx.fixed_delta);
    process_construction(dt);
    process_passive_yields(dt);
}

Entity BuildingSystem::construct_building(BuildingType type, uint32_t faction_id, Math::Vector2f position) {
    if (!m_world) return INVALID_ENTITY;

    Entity entity = m_world->CreateEntity();
    TransformComponent transform;
    transform.position = position;
    m_world->AddComponent(entity, transform);

    BuildingComponent building;
    building.building_type = type;
    building.faction_id = faction_id;
    building.health = 50.0f;
    building.max_health = 100.0f;
    building.construction_progress = 0.1f; // Starts under construction
    building.is_operational = false;
    m_world->AddComponent(entity, building);

    StorageComponent storage;
    m_world->AddComponent(entity, storage);

    return entity;
}

void BuildingSystem::repair_building(Entity entity, float amount) {
    if (!m_world || !m_world->HasComponent<BuildingComponent>(entity)) return;

    auto& building = m_world->GetComponent<BuildingComponent>(entity);
    building.health = std::min(building.max_health, building.health + amount);
    if (building.health > 0.0f && building.construction_progress >= 1.0f) {
        building.is_operational = true;
    }
}

void BuildingSystem::process_construction(float dt) {
    auto entities = m_world->Query<BuildingComponent>();
    const float CONSTRUCTION_SPEED = 0.2f; // 5 seconds to complete

    for (Entity e : entities) {
        auto& building = m_world->GetComponent<BuildingComponent>(e);
        if (building.construction_progress < 1.0f) {
            building.construction_progress = std::min(1.0f, building.construction_progress + (CONSTRUCTION_SPEED * dt));
            if (building.construction_progress >= 1.0f) {
                building.health = building.max_health;
                building.is_operational = true;
            }
        }
    }
}

void BuildingSystem::process_passive_yields(float dt) {
    auto entities = m_world->Query<BuildingComponent, StorageComponent>();

    for (Entity e : entities) {
        auto& building = m_world->GetComponent<BuildingComponent>(e);
        if (!building.is_operational || building.health <= 0.0f) continue;

        auto& storage = m_world->GetComponent<StorageComponent>(e);
        building.passive_yield_accumulator += dt;

        if (building.passive_yield_accumulator >= 1.0f) { // Yield every 1 second
            building.passive_yield_accumulator -= 1.0f;

            switch (building.building_type) {
                case BuildingType::Farm:
                    storage.biomass += 5.0f;
                    break;
                case BuildingType::Depot:
                    storage.minerals += 3.0f;
                    break;
                case BuildingType::ResearchLab:
                    storage.sciencePoints += 2.0f;
                    break;
                case BuildingType::Outpost:
                    storage.minerals += 1.0f;
                    storage.biomass += 1.0f;
                    break;
            }
        }
    }
}

} // namespace Shape::Simulation
