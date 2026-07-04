#include "AI/OptimizationSystem.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/AIComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "ECS/LifecycleComponent.hpp"
#include "ECS/CareerComponent.hpp"
#include "ECS/GeneticsComponent.hpp"
#include "ECS/ResourceComponent.hpp"
#include "ECS/MindComponent.hpp"
#include "ECS/InfluenceComponent.hpp"
#include <cmath>
#include <algorithm>

namespace Shape {

void OptimizationSystem::Update(World& world, const SpatialHashGrid& spatialGrid, f32 dt) {
    auto entities = world.Query<AIComponent, RigidBodyComponent, LifecycleComponent, CareerComponent, GeneticsComponent>();
    for (Entity entity : entities) {
        EvaluateAI(world, spatialGrid, entity, dt);
        ExecuteAI(world, entity, dt);
    }
}

void OptimizationSystem::EvaluateAI(World& world, const SpatialHashGrid& spatialGrid, Entity entity, f32 dt) {
    auto& ai = world.GetComponent<AIComponent>(entity);
    auto& trans = world.GetComponent<TransformComponent>(entity);
    auto& lifecycle = world.GetComponent<LifecycleComponent>(entity);
    auto& career = world.GetComponent<CareerComponent>(entity);
    auto& genetics = world.GetComponent<GeneticsComponent>(entity);

    ai.actionTimer -= dt;
    if (ai.actionTimer > 0.0f) return;

    // Reset decision timer
    ai.actionTimer = ai.evaluationInterval;

    // 1. Evaluate "Gather" utility
    f32 gatherUtility = 0.0f;
    Entity bestResource = INVALID_ENTITY;
    Math::Vector2f bestResourcePos = Math::Vector2f::Zero();

    // Query spatial grid for resources within a radius of 60.0 units
    std::vector<Entity> nearbyEntities;
    spatialGrid.Query(trans.position, 60.0f, nearbyEntities);

    f32 closestDistSq = 999999.0f;
    for (Entity neighbor : nearbyEntities) {
        if (neighbor == entity) continue;
        if (!world.HasComponent<ResourceComponent>(neighbor)) continue;

        auto& res = world.GetComponent<ResourceComponent>(neighbor);
        if (res.isDepleted) continue;

        auto& resTrans = world.GetComponent<TransformComponent>(neighbor);
        
        f32 distSq = (trans.position - resTrans.position).LengthSquared();
        if (distSq < closestDistSq) {
            closestDistSq = distSq;
            bestResource = neighbor;
            bestResourcePos = resTrans.position;
        }
    }

    if (bestResource != INVALID_ENTITY) {
        f32 hungerFactor = (100.0f - lifecycle.energy) / 100.0f; // Urgency
        f32 distanceFactor = 1.0f / (1.0f + 0.02f * closestDistSq);
        
        // Gene 4 represents Survival/Metabolism affinity
        gatherUtility = hungerFactor * 10.0f * distanceFactor * genetics.genome[4] * career.skillSurvival;
    } else if (world.HasComponent<MindComponent>(entity)) {
        // Fallback to memory coordinates if no resource is in direct line of sight
        auto& mind = world.GetComponent<MindComponent>(entity);
        f32 closestMemDistSq = 999999.0f;
        for (const auto& mem : mind.memories) {
            f32 distSq = (trans.position - mem.position).LengthSquared();
            if (distSq < closestMemDistSq) {
                closestMemDistSq = distSq;
                bestResourcePos = mem.position;
                closestDistSq = distSq;
            }
        }
        if (closestMemDistSq < 999999.0f) {
            f32 hungerFactor = (100.0f - lifecycle.energy) / 100.0f;
            f32 distanceFactor = 1.0f / (1.0f + 0.02f * closestDistSq);
            gatherUtility = hungerFactor * 8.0f * distanceFactor * genetics.genome[4] * career.skillSurvival;
        }
    }

    // 2. Evaluate "Explore" utility
    // Gene 9 represents Exploration affinity
    f32 exploreUtility = 1.5f * genetics.genome[9] * career.skillExploration;
    f32 escapeUtility = 0.0f;

    if (world.HasComponent<MindComponent>(entity)) {
        auto& mind = world.GetComponent<MindComponent>(entity);
        exploreUtility += mind.curiosity * mind.wonder * 0.1f;
        escapeUtility = mind.fear * 1.2f; // Fear levels multiply escape score
    }

    // 3. Evaluate "Reproduce" utility
    f32 reproduceUtility = 0.0f;
    if (lifecycle.energy > lifecycle.reproductionThreshold) {
        // High energy triggers reproduction impulse
        reproduceUtility = (lifecycle.energy - lifecycle.reproductionThreshold) * 1.5f * genetics.genome[4];
    }

    // 4. Decide best action
    f32 maxUtility = exploreUtility;
    AIAction selectedAction = AIAction::Explore;

    if (gatherUtility > maxUtility) {
        maxUtility = gatherUtility;
        selectedAction = AIAction::Gather;
    }

    if (reproduceUtility > maxUtility) {
        maxUtility = reproduceUtility;
        selectedAction = AIAction::Reproduce;
    }

    if (escapeUtility > maxUtility) {
        maxUtility = escapeUtility;
        selectedAction = AIAction::Escape;
    }

    ai.currentAction = selectedAction;

    if (selectedAction == AIAction::Gather) {
        ai.targetEntity = bestResource;
        ai.targetPosition = bestResourcePos;
    } else if (selectedAction == AIAction::Explore) {
        f32 angle = static_cast<f32>(entity & 0xFF) + trans.position.x;
        ai.targetEntity = INVALID_ENTITY;
        ai.targetPosition = trans.position + Math::Vector2f(Math::Cos(angle), Math::Sin(angle)) * 30.0f;
    } else if (selectedAction == AIAction::Escape) {
        // Find nearest fear emitter source
        Math::Vector2f threatSource = Math::Vector2f::Zero();
        f32 closestThreatDistSq = 999999.0f;
        for (Entity neighbor : nearbyEntities) {
            if (world.HasComponent<InfluenceComponent>(neighbor)) {
                auto& inf = world.GetComponent<InfluenceComponent>(neighbor);
                if (inf.type == InfluenceType::Fear) {
                    auto& tTrans = world.GetComponent<TransformComponent>(neighbor);
                    
                    f32 distSq = (trans.position - tTrans.position).LengthSquared();
                    if (distSq < closestThreatDistSq) {
                        closestThreatDistSq = distSq;
                        threatSource = tTrans.position;
                    }
                }
            }
        }
        ai.targetEntity = INVALID_ENTITY;
        if (closestThreatDistSq < 999999.0f) {
            Math::Vector2f escapeDir = (trans.position - threatSource).Normalized();
            ai.targetPosition = trans.position + escapeDir * 40.0f;
        } else {
            ai.targetPosition = trans.position - Math::Vector2f(10.0f, 10.0f);
        }
    } else {
        ai.targetEntity = INVALID_ENTITY;
        ai.targetPosition = trans.position;
    }
}

void OptimizationSystem::ExecuteAI(World& world, Entity entity, f32 dt) {
    auto& ai = world.GetComponent<AIComponent>(entity);
    auto& trans = world.GetComponent<TransformComponent>(entity);
    auto& body = world.GetComponent<RigidBodyComponent>(entity);
    auto& lifecycle = world.GetComponent<LifecycleComponent>(entity);
    auto& genetics = world.GetComponent<GeneticsComponent>(entity);

    // Apply movement forces to steer toward goal
    if (ai.currentAction == AIAction::Explore || ai.currentAction == AIAction::Gather || ai.currentAction == AIAction::Escape) {
        Math::Vector2f toTarget = ai.targetPosition - trans.position;
        f32 distSq = toTarget.LengthSquared();

        if (distSq > 1.0f) {
            Math::Vector2f dir = toTarget.Normalized();
            f32 forceAmount = genetics.genome[3]; // Max Speed Gene
            body.force += dir * forceAmount;
            
            // Kinetic energy cost
            lifecycle.energy -= 0.5f * body.velocity.Length() * dt;
        }
    }

    // Process harvesting/gather logic
    if (ai.currentAction == AIAction::Gather && ai.targetEntity != INVALID_ENTITY) {
        if (world.HasComponent<ResourceComponent>(ai.targetEntity)) {
            auto& res = world.GetComponent<ResourceComponent>(ai.targetEntity);
            auto& resTrans = world.GetComponent<TransformComponent>(ai.targetEntity);
            

            f32 dist = (trans.position - resTrans.position).Length();
            if (dist < 8.0f) { // Close enough to harvest
                f32 rate = 15.0f * dt;
                f32 harvest = std::min(rate, res.resourceAmount);
                
                res.resourceAmount -= harvest;
                lifecycle.energy += harvest;

                if (res.resourceAmount <= 0.0f) {
                    res.isDepleted = true;
                    ai.targetEntity = INVALID_ENTITY;
                    ai.currentAction = AIAction::Idle;
                }
            }
        }
    }

    // Metabolism decay
    lifecycle.energy -= lifecycle.energyLossPerSecond * dt;
    lifecycle.energy = std::clamp(lifecycle.energy, 0.0f, 100.0f);

    if (lifecycle.energy <= 0.0f) {
        lifecycle.health -= 8.0f * dt; // Starving
    } else if (lifecycle.energy > 60.0f) {
        lifecycle.health += 4.0f * dt; // Healing
    }

    lifecycle.health = std::clamp(lifecycle.health, 0.0f, 100.0f);
    if (lifecycle.health <= 0.0f) {
        lifecycle.isDead = true;
    }
}

} // namespace Shape
