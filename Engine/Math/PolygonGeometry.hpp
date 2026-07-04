#pragma once

#include "ShapeEngine/Math/Vector2.hpp"
#include <vector>

namespace Shape::Math {

class PolygonGeometry {
public:
    // Procedural generation of a regular N-gon centered at (0, 0)
    static std::vector<Vector2f> CreateRegularPolygon(u32 sideCount, f32 radius);

    // Bounding circle calculation
    static f32 ComputeBoundingRadius(const Vector2f* vertices, usize count);

    // Mass and Inertia estimators
    static f32 ComputeArea(const Vector2f* vertices, usize count);
    static f32 ComputePerimeter(const Vector2f* vertices, usize count);
    static Vector2f ComputeCentroid(const Vector2f* vertices, usize count);
    static f32 ComputeInertia(const Vector2f* vertices, usize count, f32 mass);
};

} // namespace Shape::Math
