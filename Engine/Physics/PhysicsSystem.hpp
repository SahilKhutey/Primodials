#pragma once

#include "ECS/World.hpp"
#include "Physics/SpatialHashGrid.hpp"

namespace Shape {

struct CollisionManifold {
    Entity entityA = INVALID_ENTITY;
    Entity entityB = INVALID_ENTITY;
    Math::Vector2f normal = Math::Vector2f::Zero();
    f32 penetration = 0.0f;
    Math::Vector2f contactPoint = Math::Vector2f::Zero();
    bool colliding = false;
};

class PhysicsSystem {
public:
    explicit PhysicsSystem(f32 cellSize = 10.0f);
    ~PhysicsSystem() = default;

    void Update(World& world, f32 dt);

    // Dynamic collision checks
    static CollisionManifold CheckCircleCollision(
        Entity entityA, const Math::Vector2f& posA, f32 radA,
        Entity entityB, const Math::Vector2f& posB, f32 radB
    );

    static CollisionManifold CheckPolygonCollision(
        Entity entityA, const Math::Vector2f& posA, const Math::Vector2f* verticesA, usize countA,
        Entity entityB, const Math::Vector2f& posB, const Math::Vector2f* verticesB, usize countB
    );

    static CollisionManifold CheckCirclePolygonCollision(
        Entity entityCircle, const Math::Vector2f& center, f32 radius,
        Entity entityPoly, const Math::Vector2f& polyPos, const Math::Vector2f* vertices, usize count
    );

private:
    void IntegrateKinematics(World& world, f32 dt);
    void BroadPhaseCollision(World& world, std::vector<std::pair<Entity, Entity>>& outPairs);
    void NarrowPhaseCollision(World& world, const std::vector<std::pair<Entity, Entity>>& pairs, std::vector<CollisionManifold>& outCollisions);
    void ResolveCollisions(World& world, const std::vector<CollisionManifold>& collisions);

    SpatialHashGrid m_Grid;
};

} // namespace Shape
