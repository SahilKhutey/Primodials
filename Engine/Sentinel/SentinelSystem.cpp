#include "Sentinel/SentinelSystem.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/SentinelComponent.hpp"
#include "ECS/InfluenceComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "ECS/ResourceComponent.hpp"
#include "ECS/MindComponent.hpp"

namespace Shape {

void SentinelSystem::Update(World& world, const SpatialHashGrid& spatialGrid, f32 dt) {
    auto sentinels = world.Query<SentinelComponent, RigidBodyComponent>();

    for (Entity entity : sentinels) {
        auto& sentinel = world.GetComponent<SentinelComponent>(entity);
        auto& trans = world.GetComponent<TransformComponent>(entity);
        

        // 1. Ensure Sentinel projects the correct influence field
        if (!world.HasComponent<InfluenceComponent>(entity)) {
            InfluenceComponent emitter;
            emitter.radius = sentinel.commandRadius;
            
            switch (sentinel.type) {
                case SentinelType::Military:
                case SentinelType::Construction:
                    emitter.type = InfluenceType::Leadership;
                    emitter.strength = 20.0f;
                    break;
                case SentinelType::Science:
                    emitter.type = InfluenceType::Knowledge;
                    emitter.strength = 25.0f;
                    break;
                case SentinelType::Economic:
                    emitter.type = InfluenceType::Leadership;
                    emitter.strength = 15.0f;
                    break;
            }
            world.AddComponent<InfluenceComponent>(entity, emitter);
        }

        // 2. Process distributed memory sync (Knowledge Network)
        sentinel.syncTimer -= dt;
        if (sentinel.syncTimer <= 0.0f) {
            sentinel.syncTimer = sentinel.syncInterval;

            // Query spatial hash grid for nearby resources and shapes
            std::vector<Entity> nearbyEntities;
            spatialGrid.Query(trans.position, sentinel.commandRadius, nearbyEntities);

            std::vector<Entity> resourceNodes;
            std::vector<Entity> localMinds;

            for (Entity neighbor : nearbyEntities) {
                if (world.HasComponent<ResourceComponent>(neighbor)) {
                    auto& res = world.GetComponent<ResourceComponent>(neighbor);
                    if (!res.isDepleted) {
                        resourceNodes.push_back(neighbor);
                    }
                }
                if (world.HasComponent<MindComponent>(neighbor)) {
                    localMinds.push_back(neighbor);
                }
            }

            // Sync resource coordinate locations to nearby shapes
            for (Entity shape : localMinds) {
                auto& mind = world.GetComponent<MindComponent>(shape);
                
                for (Entity resource : resourceNodes) {
                    
                    auto& res = world.GetComponent<ResourceComponent>(resource);
                    
                    auto& resTrans = world.GetComponent<TransformComponent>(resource);
                    mind.AddMemory(resTrans.position, res.resourceAmount);
                }
            }
        }
    }
}

} // namespace Shape
