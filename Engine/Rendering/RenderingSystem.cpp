#include "RenderingSystem.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include "Physics/ColliderComponent.hpp"
#include <vector>

namespace Shape {

namespace {
    Math::Vector2f Lerp(const Math::Vector2f& a, const Math::Vector2f& b, f32 alpha) {
        return a * (1.0f - alpha) + b * alpha;
    }

    f32 LerpFloat(f32 a, f32 b, f32 alpha) {
        return a * (1.0f - alpha) + b * alpha;
    }

    Math::Vector2f RotateVertex(const Math::Vector2f& vertex, f32 radians) {
        f32 c = Math::Cos(radians);
        f32 s = Math::Sin(radians);
        return Math::Vector2f(vertex.x * c - vertex.y * s, vertex.x * s + vertex.y * c);
    }
}

void RenderingSystem::Render(World& world, IRenderer& renderer, f32 alpha) {
    // Render Polygons
    auto polyEntities = world.Query<TransformComponent, GeometryComponent>();
    for (Entity entity : polyEntities) {
        auto& trans = world.GetComponent<TransformComponent>(entity);
        auto& geom = world.GetComponent<GeometryComponent>(entity);

        Math::Vector2f visualPos = Lerp(trans.previousPosition, trans.position, alpha);
        f32 visualRot = LerpFloat(trans.previousRotation, trans.rotation, alpha);

        std::vector<Math::Vector2f> worldVerts(geom.vertices.size());
        for (usize i = 0; i < geom.vertices.size(); ++i) {
            worldVerts[i] = visualPos + RotateVertex(geom.vertices[i] * trans.scale.x, visualRot); // Assuming uniform scale
        }

        // Draw with some base color, could fetch from a MaterialComponent later
        renderer.DrawPolygon(worldVerts.data(), worldVerts.size(), {0.0f, 1.0f, 0.5f}, true);
        renderer.DrawPolygon(worldVerts.data(), worldVerts.size(), {1.0f, 1.0f, 1.0f}, false); // outline
    }

    // Render Circles (entities with Collider but no Geometry)
    auto colEntities = world.Query<TransformComponent, ColliderComponent>();
    for (Entity entity : colEntities) {
        if (world.HasComponent<GeometryComponent>(entity)) {
            continue; // Already drawn as polygon
        }

        auto& trans = world.GetComponent<TransformComponent>(entity);
        auto& col = world.GetComponent<ColliderComponent>(entity);

        if (col.type == ColliderType::Circle) {
            Math::Vector2f visualPos = Lerp(trans.previousPosition, trans.position, alpha);
            renderer.DrawCircle(visualPos, col.radius * trans.scale.x, {1.0f, 0.2f, 0.2f}, true);
        }
    }
}

} // namespace Shape
