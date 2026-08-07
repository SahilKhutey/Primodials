#include "Biology/BiologySystem.hpp"
#include "ECS/LifecycleComponent.hpp"
#include "ECS/TransformComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "Physics/ColliderComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include "ECS/GeneticsComponent.hpp"
#include "ECS/CareerComponent.hpp"
#include "ECS/AIComponent.hpp"
#include "ECS/ResourceComponent.hpp"
#include <cmath>

namespace Shape {

void BiologySystem::Update(World& world, CommandBuffer& cmd, f32 dt, u32 frameSeed) {
    ProcessLifecycles(world, cmd, dt);
    ProcessReproduction(world, cmd, frameSeed);
}

void BiologySystem::ProcessLifecycles(World& world, CommandBuffer& cmd, f32 dt) {
    auto entities = world.Query<TransformComponent, LifecycleComponent, RigidBodyComponent, ColliderComponent>();
    for (Entity entity : entities) {
        auto& lifecycle = world.GetComponent<LifecycleComponent>(entity);
        auto& trans = world.GetComponent<TransformComponent>(entity);
        lifecycle.age += dt;

        // Age-related health decay if exceeded maximum lifespan
        if (lifecycle.age > lifecycle.maxAge) {
            lifecycle.health -= 15.0f * dt;
        }

        if (lifecycle.health <= 0.0f) {
            lifecycle.isDead = true;
        }

        if (lifecycle.isDead) {
            Math::Vector2f deadPos = trans.position;

            // Spawn a food node at the position of death (conservation of mass)
            cmd.PushCommand([&world, deadPos]() {
                Entity food = world.CreateEntity();
                world.AddComponent<TransformComponent>(food, TransformComponent());
                world.AddComponent<RigidBodyComponent>(food, RigidBodyComponent());
                world.AddComponent<ColliderComponent>(food, ColliderComponent());
                world.AddComponent<ResourceComponent>(food, ResourceComponent());

                auto& fTrans = world.GetComponent<TransformComponent>(food);
                fTrans.position = deadPos;
                auto& fBody = world.GetComponent<RigidBodyComponent>(food);
                fBody.SetMass(0.0f); // Static food node

                auto& fCol = world.GetComponent<ColliderComponent>(food);
                fCol.type = ColliderType::Circle;
                fCol.radius = 4.0f;
            });

            // Queue entity destruction
            cmd.DestroyEntityDeferred(entity);
        }
    }
}

void BiologySystem::ProcessReproduction(World& world, CommandBuffer& cmd, u32 seed) {
    auto entities = world.Query<TransformComponent, AIComponent, LifecycleComponent, GeneticsComponent, RigidBodyComponent, ColliderComponent>();
    u32 offsetSeed = seed;

    for (Entity entity : entities) {
        auto& ai = world.GetComponent<AIComponent>(entity);
        auto& lifecycle = world.GetComponent<LifecycleComponent>(entity);
        auto& genetics = world.GetComponent<GeneticsComponent>(entity);
        auto& trans = world.GetComponent<TransformComponent>(entity);
        auto& col = world.GetComponent<ColliderComponent>(entity);

        if (ai.currentAction == AIAction::Reproduce) {
            // Deduct reproduction energy cost from parent
            lifecycle.energy = 30.0f;
            ai.currentAction = AIAction::Idle;

            Math::Vector2f parentPos = trans.position;
            f32 parentRadius = col.radius;
            GeneticsComponent parentGenetics = genetics;
            u32 parentGen = lifecycle.generation;

            // Spawn mutated child deferred
            cmd.PushCommand([&world, parentGenetics, parentPos, parentRadius, parentGen, offsetSeed]() {
                Entity child = world.CreateEntity();

                GeneticsComponent childGenetics = parentGenetics;
                DeterministicRng rng(offsetSeed);
                childGenetics.Mutate(rng);

                // Child side count procedurally morphs according to its inherited genes
                u32 sideCount = static_cast<u32>(std::round(childGenetics.genome[0]));
                f32 radius = 5.0f * childGenetics.genome[1];
                GeometryComponent childGeom(sideCount, radius);

                // Setup position with offset to prevent overlap issues
                f32 angle = static_cast<f32>(offsetSeed % 360) * 3.14159f / 180.0f;
                RigidBodyComponent childBody;
                TransformComponent childTrans;
                childTrans.position = parentPos + Math::Vector2f(std::cos(angle), std::sin(angle)) * (parentRadius + radius + 3.0f);
                childBody.SetMass(childGeom.area * 0.1f);

                ColliderComponent childCol;
                childCol.type = ColliderType::Polygon;
                childCol.radius = radius;
                childCol.restitution = childGenetics.genome[2];

                LifecycleComponent childLife;
                childLife.maxAge = childGenetics.genome[11];
                childLife.energy = 35.0f;
                childLife.generation = parentGen + 1;

                CareerComponent childCareer;
                childCareer.interestSurvival = childGenetics.genome[4];
                childCareer.interestEngineering = childGenetics.genome[5];
                childCareer.interestScience = childGenetics.genome[6];
                childCareer.interestEconomy = childGenetics.genome[7];
                childCareer.interestMilitary = childGenetics.genome[8];
                childCareer.interestExploration = childGenetics.genome[9];

                world.AddComponent<TransformComponent>(child, childTrans);
                world.AddComponent<RigidBodyComponent>(child, childBody);
                world.AddComponent<ColliderComponent>(child, childCol);
                world.AddComponent<GeometryComponent>(child, childGeom);
                world.AddComponent<GeneticsComponent>(child, childGenetics);
                world.AddComponent<LifecycleComponent>(child, childLife);
                world.AddComponent<CareerComponent>(child, childCareer);
                world.AddComponent<AIComponent>(child, AIComponent());
            });

            offsetSeed += 31; // Increment offset seed for subsequent entities
        }
    }
}

} // namespace Shape
