#include "Physics/PhysicsSystem.hpp"
#include "Jobs/JobSystem.hpp"
#include "ECS/TransformComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "Physics/ColliderComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

namespace Shape {

namespace {
    // Helper to rotate a local vertex to world space
    Math::Vector2f RotateVertex(const Math::Vector2f& vertex, f32 radians) {
        f32 c = Math::Cos(radians);
        f32 s = Math::Sin(radians);
        return Math::Vector2f(vertex.x * c - vertex.y * s, vertex.x * s + vertex.y * c);
    }

    // Helper to project a polygon onto an axis
    void ProjectPolygon(const Math::Vector2f* vertices, usize count, const Math::Vector2f& axis, f32& min, f32& max) {
        min = vertices[0].Dot(axis);
        max = min;
        for (usize i = 1; i < count; ++i) {
            f32 projection = vertices[i].Dot(axis);
            if (projection < min) min = projection;
            if (projection > max) max = projection;
        }
    }

    // Helper to project a circle onto an axis
    void ProjectCircle(const Math::Vector2f& center, f32 radius, const Math::Vector2f& axis, f32& min, f32& max) {
        f32 projCenter = center.Dot(axis);
        min = projCenter - radius;
        max = projCenter + radius;
    }
}

PhysicsSystem::PhysicsSystem(f32 cellSize)
    : m_Grid(cellSize) {
}

void PhysicsSystem::Update(World& world, f32 dt) {
    IntegrateKinematics(world, dt);

    std::vector<std::pair<Entity, Entity>> pairs;
    BroadPhaseCollision(world, pairs);

    std::vector<CollisionManifold> collisions;
    NarrowPhaseCollision(world, pairs, collisions);

    ResolveCollisions(world, collisions);
}

void PhysicsSystem::IntegrateKinematics(World& world, f32 dt) {
    auto entities = world.Query<TransformComponent, RigidBodyComponent>();
    if (entities.empty()) return;

    // Use JobSystem to parallelize the integration
    shape::JobSystem::Get().ParallelFor(entities.size(), 256, [&world, &entities, dt](u32 start, u32 end) {
        for (u32 i = start; i < end; ++i) {
            Entity entity = entities[i];
            auto& trans = world.GetComponent<TransformComponent>(entity);
            auto& body = world.GetComponent<RigidBodyComponent>(entity);
            
            if (body.inverseMass <= 0.0f) continue;

            trans.previousPosition = trans.position;
            trans.previousRotation = trans.rotation;

            body.velocity += (body.force * body.inverseMass) * dt;
            trans.position += body.velocity * dt;

            body.angularVelocity += (body.torque * body.inverseInertia) * dt;
            trans.rotation += body.angularVelocity * dt;

            body.velocity *= (1.0f - body.linearDamping);
            body.angularVelocity *= (1.0f - body.angularDamping);

            body.force = Math::Vector2f::Zero();
            body.torque = 0.0f;
        }
    });
}

void PhysicsSystem::BroadPhaseCollision(World& world, std::vector<std::pair<Entity, Entity>>& outPairs) {
    m_Grid.Clear();
    auto entities = world.Query<TransformComponent, RigidBodyComponent, ColliderComponent>();

    // Insert all active colliders into the spatial grid
    for (Entity entity : entities) {
        auto& trans = world.GetComponent<TransformComponent>(entity);
        auto& collider = world.GetComponent<ColliderComponent>(entity);

        f32 size = collider.radius;
        if (collider.type == ColliderType::Polygon && world.HasComponent<GeometryComponent>(entity)) {
            auto& geom = world.GetComponent<GeometryComponent>(entity);
            size = geom.GetBoundingRadius();
        }

        m_Grid.Insert(entity, trans.position, size);
    }

    // Query spatial grid for nearby colliders
    for (Entity entityA : entities) {
        auto& transA = world.GetComponent<TransformComponent>(entityA);
        auto& bodyA = world.GetComponent<RigidBodyComponent>(entityA);
        auto& colA = world.GetComponent<ColliderComponent>(entityA);

        f32 sizeA = colA.radius;
        if (colA.type == ColliderType::Polygon && world.HasComponent<GeometryComponent>(entityA)) {
            sizeA = world.GetComponent<GeometryComponent>(entityA).GetBoundingRadius();
        }

        std::vector<Entity> neighbors;
        m_Grid.Query(transA.position, sizeA, neighbors);

        for (Entity entityB : neighbors) {
            // Avoid duplicates, self-collision, and static-static pairs
            if (entityA >= entityB) continue;

            auto& bodyB = world.GetComponent<RigidBodyComponent>(entityB);
            if (bodyA.inverseMass == 0.0f && bodyB.inverseMass == 0.0f) continue;

            outPairs.push_back({entityA, entityB});
        }
    }
}

void PhysicsSystem::NarrowPhaseCollision(
    World& world, 
    const std::vector<std::pair<Entity, Entity>>& pairs, 
    std::vector<CollisionManifold>& outCollisions
) {
    for (const auto& pair : pairs) {
        Entity entityA = pair.first;
        Entity entityB = pair.second;

        auto& transA = world.GetComponent<TransformComponent>(entityA);
        auto& transB = world.GetComponent<TransformComponent>(entityB);

        auto& colA = world.GetComponent<ColliderComponent>(entityA);
        auto& colB = world.GetComponent<ColliderComponent>(entityB);

        CollisionManifold manifold;

        if (colA.type == ColliderType::Circle && colB.type == ColliderType::Circle) {
            manifold = CheckCircleCollision(entityA, transA.position, colA.radius, entityB, transB.position, colB.radius);
        }
        else if (colA.type == ColliderType::Polygon && colB.type == ColliderType::Polygon) {
            if (world.HasComponent<GeometryComponent>(entityA) && world.HasComponent<GeometryComponent>(entityB)) {
                auto& geomA = world.GetComponent<GeometryComponent>(entityA);
                auto& geomB = world.GetComponent<GeometryComponent>(entityB);

                // Build world-space vertices
                std::vector<Math::Vector2f> worldVertsA(geomA.vertices.size());
                std::vector<Math::Vector2f> worldVertsB(geomB.vertices.size());

                for (usize i = 0; i < geomA.vertices.size(); ++i) {
                    worldVertsA[i] = transA.position + RotateVertex(geomA.vertices[i], transA.rotation);
                }
                for (usize i = 0; i < geomB.vertices.size(); ++i) {
                    worldVertsB[i] = transB.position + RotateVertex(geomB.vertices[i], transB.rotation);
                }

                manifold = CheckPolygonCollision(
                    entityA, transA.position, worldVertsA.data(), worldVertsA.size(),
                    entityB, transB.position, worldVertsB.data(), worldVertsB.size()
                );
            }
        }
        else {
            // One circle, one polygon
            Entity entityCircle = colA.type == ColliderType::Circle ? entityA : entityB;
            Entity entityPoly = colA.type == ColliderType::Polygon ? entityA : entityB;

            auto& transCircle = world.GetComponent<TransformComponent>(entityCircle);
            
            auto& transPoly = world.GetComponent<TransformComponent>(entityPoly);
            
            auto& colCircle = world.GetComponent<ColliderComponent>(entityCircle);

            if (world.HasComponent<GeometryComponent>(entityPoly)) {
                auto& geom = world.GetComponent<GeometryComponent>(entityPoly);
                std::vector<Math::Vector2f> worldVerts(geom.vertices.size());

                for (usize i = 0; i < geom.vertices.size(); ++i) {
                    worldVerts[i] = transPoly.position + RotateVertex(geom.vertices[i], transPoly.rotation);
                }

                manifold = CheckCirclePolygonCollision(
                    entityCircle, transCircle.position, colCircle.radius,
                    entityPoly, transPoly.position, worldVerts.data(), worldVerts.size()
                );
            }
        }

        if (manifold.colliding) {
            outCollisions.push_back(manifold);
        }
    }
}

CollisionManifold PhysicsSystem::CheckCircleCollision(
    Entity entityA, const Math::Vector2f& posA, f32 radA,
    Entity entityB, const Math::Vector2f& posB, f32 radB
) {
    CollisionManifold manifold;
    manifold.entityA = entityA;
    manifold.entityB = entityB;

    Math::Vector2f dist = posB - posA;
    f32 distLenSq = dist.LengthSquared();
    f32 radSum = radA + radB;

    if (distLenSq >= radSum * radSum) {
        return manifold; // No collision
    }

    f32 distLen = Math::Sqrt(distLenSq);
    manifold.colliding = true;
    manifold.penetration = radSum - distLen;

    if (distLen > Math::EPSILON) {
        manifold.normal = dist / distLen;
    } else {
        // Overlapping at the exact center coordinates
        manifold.normal = Math::Vector2f(1.0f, 0.0f);
    }

    manifold.contactPoint = posA + manifold.normal * radA;
    return manifold;
}

CollisionManifold PhysicsSystem::CheckPolygonCollision(
    Entity entityA, const Math::Vector2f& posA, const Math::Vector2f* verticesA, usize countA,
    Entity entityB, const Math::Vector2f& posB, const Math::Vector2f* verticesB, usize countB
) {
    CollisionManifold manifold;
    manifold.colliding = false;
    manifold.entityA = entityA;
    manifold.entityB = entityB;

    f32 minOverlap = std::numeric_limits<f32>::max();
    Math::Vector2f smallestAxis = Math::Vector2f::Zero();

    // Check face normals of A and B
    for (usize i = 0; i < countA + countB; ++i) {
        Math::Vector2f edge;
        if (i < countA) {
            edge = verticesA[(i + 1) % countA] - verticesA[i];
        } else {
            usize j = i - countA;
            edge = verticesB[(j + 1) % countB] - verticesB[j];
        }

        // Normal of the edge
        Math::Vector2f axis = Math::Vector2f(-edge.y, edge.x).Normalized();

        f32 minA, maxA;
        f32 minB, maxB;

        ProjectPolygon(verticesA, countA, axis, minA, maxA);
        ProjectPolygon(verticesB, countB, axis, minB, maxB);

        // Gap check
        if (maxA < minB || maxB < minA) {
            return manifold; // Separating axis found
        }

        f32 overlap = std::min(maxA, maxB) - std::max(minA, minB);
        if (overlap < minOverlap) {
            minOverlap = overlap;
            smallestAxis = axis;
        }
    }

    // Direction check (normal must point from A to B)
    Math::Vector2f d = posB - posA;
    if (d.Dot(smallestAxis) < 0.0f) {
        smallestAxis = -smallestAxis;
    }

    manifold.colliding = true;
    manifold.normal = smallestAxis;
    manifold.penetration = minOverlap;
    manifold.contactPoint = posA + smallestAxis * (minOverlap * 0.5f); // Approximation
    return manifold;
}

CollisionManifold PhysicsSystem::CheckCirclePolygonCollision(
    Entity entityCircle, const Math::Vector2f& center, f32 radius,
    Entity entityPoly, const Math::Vector2f& polyPos, const Math::Vector2f* vertices, usize count
) {
    CollisionManifold manifold;
    manifold.colliding = false;
    manifold.entityA = entityCircle;
    manifold.entityB = entityPoly;

    f32 minOverlap = std::numeric_limits<f32>::max();
    Math::Vector2f smallestAxis = Math::Vector2f::Zero();

    // Find axis pointing from the closest vertex to the circle center (critical for circle vs vertex collisions)
    f32 minVertexDistSq = std::numeric_limits<f32>::max();
    Math::Vector2f closestVertex = Math::Vector2f::Zero();

    for (usize i = 0; i < count; ++i) {
        f32 distSq = (center - vertices[i]).LengthSquared();
        if (distSq < minVertexDistSq) {
            minVertexDistSq = distSq;
            closestVertex = vertices[i];
        }
    }
    
    Math::Vector2f closestVertexAxis = (center - closestVertex).Normalized();

    // Check edge normals + closest vertex axis
    for (usize i = 0; i <= count; ++i) {
        Math::Vector2f axis;
        if (i < count) {
            Math::Vector2f edge = vertices[(i + 1) % count] - vertices[i];
            axis = Math::Vector2f(-edge.y, edge.x).Normalized();
        } else {
            axis = closestVertexAxis;
        }

        if (axis.LengthSquared() < Math::EPSILON) continue;

        f32 minC, maxC;
        f32 minP, maxP;

        ProjectCircle(center, radius, axis, minC, maxC);
        ProjectPolygon(vertices, count, axis, minP, maxP);

        // Gap check
        if (maxC < minP || maxP < minC) {
            return manifold; // Separating axis found
        }

        f32 overlap = std::min(maxC, maxP) - std::max(minC, minP);
        if (overlap < minOverlap) {
            minOverlap = overlap;
            smallestAxis = axis;
        }
    }

    // Direction check
    Math::Vector2f d = polyPos - center;
    if (d.Dot(smallestAxis) < 0.0f) {
        smallestAxis = -smallestAxis;
    }

    manifold.colliding = true;
    manifold.normal = smallestAxis;
    manifold.penetration = minOverlap;
    manifold.contactPoint = center + smallestAxis * radius;
    return manifold;
}

void PhysicsSystem::ResolveCollisions(World& world, const std::vector<CollisionManifold>& collisions) {
    for (const auto& manifold : collisions) {
        auto& transA = world.GetComponent<TransformComponent>(manifold.entityA);
        auto& bodyA = world.GetComponent<RigidBodyComponent>(manifold.entityA);
        auto& transB = world.GetComponent<TransformComponent>(manifold.entityB);
        auto& bodyB = world.GetComponent<RigidBodyComponent>(manifold.entityB);

        auto& colA = world.GetComponent<ColliderComponent>(manifold.entityA);
        auto& colB = world.GetComponent<ColliderComponent>(manifold.entityB);

        // 1. Resolve relative linear velocities
        Math::Vector2f rv = bodyB.velocity - bodyA.velocity;
        f32 velAlongNormal = rv.Dot(manifold.normal);

        // Do not resolve if objects are moving apart
        if (velAlongNormal > 0.0f) continue;

        // Bounciness (Restitution)
        f32 e = std::min(colA.restitution, colB.restitution);

        // Impulse scalar
        f32 totalInverseMass = bodyA.inverseMass + bodyB.inverseMass;
        if (totalInverseMass <= 0.0f) continue; // Both are static

        f32 j = -(1.0f + e) * velAlongNormal;
        j /= totalInverseMass;

        // Apply linear impulses
        bodyA.velocity -= (manifold.normal * j) * bodyA.inverseMass;
        bodyB.velocity += (manifold.normal * j) * bodyB.inverseMass;

        // 2. Positional Correction to prevent sinking/jittering (linear projection)
        const f32 percent = 0.4f; // Penetration percentage to resolve per frame
        const f32 slop = 0.01f;   // Penetration allowance
        Math::Vector2f correction = manifold.normal * ((std::max(manifold.penetration - slop, 0.0f) / totalInverseMass) * percent);
        
        transA.position -= correction * bodyA.inverseMass;
        transB.position += correction * bodyB.inverseMass;
    }
}

} // namespace Shape
