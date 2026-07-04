#pragma once

#include "ShapeEngine/Math/Vector2.hpp"
#include "Math/PolygonGeometry.hpp"
#include <vector>

namespace Shape {

struct GeometryComponent {
    std::vector<Math::Vector2f> vertices;
    u32 sideCount = 3;
    f32 scale = 1.0f;

    // Cached properties
    f32 boundingRadius = 0.0f;
    f32 area = 0.0f;
    f32 perimeter = 0.0f;
    Math::Vector2f centroid = Math::Vector2f::Zero();

    GeometryComponent() = default;

    GeometryComponent(u32 sides, f32 radius, f32 scaleFactor = 1.0f)
        : sideCount(sides)
        , scale(scaleFactor) {
        
        vertices = Math::PolygonGeometry::CreateRegularPolygon(sides, radius * scaleFactor);
        RecalculateCache();
    }

    void RecalculateCache() {
        boundingRadius = Math::PolygonGeometry::ComputeBoundingRadius(vertices.data(), vertices.size());
        area = Math::PolygonGeometry::ComputeArea(vertices.data(), vertices.size());
        perimeter = Math::PolygonGeometry::ComputePerimeter(vertices.data(), vertices.size());
        centroid = Math::PolygonGeometry::ComputeCentroid(vertices.data(), vertices.size());
    }

    f32 GetBoundingRadius() const { return boundingRadius; }
};

} // namespace Shape
