#include "Math/PolygonGeometry.hpp"
#include "Core/Assert.hpp"
#include <cmath>

namespace Shape::Math {

std::vector<Vector2f> PolygonGeometry::CreateRegularPolygon(u32 sideCount, f32 radius) {
    SHAPE_ASSERT(sideCount >= 3);
    std::vector<Vector2f> vertices(sideCount);

    // Procedural layout spacing
    f32 angleStep = TWO_PI / static_cast<f32>(sideCount);
    for (u32 i = 0; i < sideCount; ++i) {
        f32 currentAngle = static_cast<f32>(i) * angleStep;
        // Start vertex pointing upwards (cos for Y, sin for X to align)
        vertices[i] = Vector2f(radius * Sin(currentAngle), radius * Cos(currentAngle));
    }
    return vertices;
}

f32 PolygonGeometry::ComputeBoundingRadius(const Vector2f* vertices, usize count) {
    f32 maxDistSq = 0.0f;
    for (usize i = 0; i < count; ++i) {
        f32 distSq = vertices[i].LengthSquared();
        if (distSq > maxDistSq) {
            maxDistSq = distSq;
        }
    }
    return Sqrt(maxDistSq);
}

f32 PolygonGeometry::ComputeArea(const Vector2f* vertices, usize count) {
    if (count < 3) return 0.0f;

    f32 area = 0.0f;
    for (usize i = 0; i < count; ++i) {
        const Vector2f& v1 = vertices[i];
        const Vector2f& v2 = vertices[(i + 1) % count];
        area += (v1.x * v2.y) - (v2.x * v1.y);
    }
    return Abs(area * 0.5f);
}

f32 PolygonGeometry::ComputePerimeter(const Vector2f* vertices, usize count) {
    if (count < 2) return 0.0f;

    f32 perimeter = 0.0f;
    for (usize i = 0; i < count; ++i) {
        perimeter += (vertices[i] - vertices[(i + 1) % count]).Length();
    }
    return perimeter;
}

Vector2f PolygonGeometry::ComputeCentroid(const Vector2f* vertices, usize count) {
    if (count < 3) return Vector2f::Zero();

    Vector2f centroid(0.0f, 0.0f);
    f32 signedArea = 0.0f;

    for (usize i = 0; i < count; ++i) {
        const Vector2f& v0 = vertices[i];
        const Vector2f& v1 = vertices[(i + 1) % count];
        
        f32 a = (v0.x * v1.y) - (v1.x * v0.y);
        signedArea += a;
        
        centroid.x += (v0.x + v1.x) * a;
        centroid.y += (v0.y + v1.y) * a;
    }

    if (Abs(signedArea) < EPSILON) {
        return vertices[0];
    }

    centroid.x /= (3.0f * signedArea);
    centroid.y /= (3.0f * signedArea);
    return centroid;
}

f32 PolygonGeometry::ComputeInertia(const Vector2f* vertices, usize count, f32 mass) {
    if (count < 3 || mass <= 0.0f) return 0.0f;

    // Exact formula for moment of inertia of any polygon about its centroid
    f32 numerator = 0.0f;
    f32 denominator = 0.0f;

    for (usize i = 0; i < count; ++i) {
        const Vector2f& v1 = vertices[i];
        const Vector2f& v2 = vertices[(i + 1) % count];

        f32 factor = Abs((v1.x * v2.y) - (v2.x * v1.y));
        numerator += factor * (v1.LengthSquared() + v1.Dot(v2) + v2.LengthSquared());
        denominator += factor;
    }

    if (denominator < EPSILON) return 0.0f;

    return (mass / 6.0f) * (numerator / denominator);
}

} // namespace Shape::Math
