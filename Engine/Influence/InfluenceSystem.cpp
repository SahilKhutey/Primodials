#include "Influence/InfluenceSystem.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/MindComponent.hpp"
#include "ECS/InfluenceComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"

namespace Shape {

void InfluenceSystem::Update(World& world, const SpatialHashGrid& spatialGrid) {
    auto receivers = world.Query<MindComponent, RigidBodyComponent>();

    // 1. Reset all accumulated field buffers
    for (Entity entity : receivers) {
        auto& mind = world.GetComponent<MindComponent>(entity);
        mind.accumulatedFearField = 0.0f;
        mind.accumulatedLeadershipField = 0.0f;
        mind.accumulatedKnowledgeField = 0.0f;
    }

    // 2. Query spatial hash grid and accumulate local emissions
    for (Entity receiver : receivers) {
        auto& mind = world.GetComponent<MindComponent>(receiver);
        auto& trans = world.GetComponent<TransformComponent>(receiver);
        

        // Scan neighbors within a maximum possible influence radius (e.g., 60.0 units)
        std::vector<Entity> neighbors;
        spatialGrid.Query(trans.position, 60.0f, neighbors);

        for (Entity emitter : neighbors) {
            if (emitter == receiver) continue;
            if (!world.HasComponent<InfluenceComponent>(emitter)) continue;

            auto& influence = world.GetComponent<InfluenceComponent>(emitter);
            auto& emitterTrans = world.GetComponent<TransformComponent>(emitter);
                

            f32 distSq = (trans.position - emitterTrans.position).LengthSquared();
            f32 maxDistSq = influence.radius * influence.radius;

            if (distSq <= maxDistSq) {
                // Attenuation: 1.0f / (1.0f + distance^2)
                f32 strengthFactor = influence.strength / (1.0f + distSq);

                switch (influence.type) {
                    case InfluenceType::Fear:
                        mind.accumulatedFearField += strengthFactor;
                        break;
                    case InfluenceType::Leadership:
                        mind.accumulatedLeadershipField += strengthFactor;
                        break;
                    case InfluenceType::Knowledge:
                        mind.accumulatedKnowledgeField += strengthFactor;
                        break;
                }
            }
        }

        // Apply accumulated field values directly to emotions as emotional drivers
        mind.fear += mind.accumulatedFearField * 5.0f; // Scale factor
        mind.fear = std::min(mind.fear, 100.0f);
        
        mind.trust += mind.accumulatedLeadershipField * 2.0f;
        mind.trust = std::min(mind.trust, 100.0f);
    }
}

} // namespace Shape
