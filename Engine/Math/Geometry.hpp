#pragma once

#include "Math/Vec2.hpp"
#include "Math/Vec3.hpp"
#include "Math/MathUtils.hpp"
#include <vector>
#include <span>

namespace Shape::Math {

// ─────────────────────────────────────────────────────────────────────────────
//  Axis-Aligned Bounding Box (2-D and 3-D)
// ─────────────────────────────────────────────────────────────────────────────
struct AABB2D {
    Vec2f min{  1e30f,  1e30f };
    Vec2f max{ -1e30f, -1e30f };

    static AABB2D FromCenterHalfSize(Vec2f center, Vec2f half) {
        return { center - half, center + half };
    }
    static AABB2D FromPoints(std::span<const Vec2f> pts) {
        AABB2D box;
        for (auto& p : pts) {
            box.min.x = Min(box.min.x, p.x);  box.min.y = Min(box.min.y, p.y);
            box.max.x = Max(box.max.x, p.x);  box.max.y = Max(box.max.y, p.y);
        }
        return box;
    }

    Vec2f Center()  const { return (min + max) * 0.5f; }
    Vec2f HalfSize() const { return (max - min) * 0.5f; }
    Vec2f Size()    const { return max - min; }
    bool  IsValid() const { return min.x <= max.x && min.y <= max.y; }

    bool Contains(Vec2f p) const {
        return p.x >= min.x && p.x <= max.x && p.y >= min.y && p.y <= max.y;
    }
    bool Overlaps(const AABB2D& o) const {
        return min.x <= o.max.x && max.x >= o.min.x &&
               min.y <= o.max.y && max.y >= o.min.y;
    }
    AABB2D Expanded(f32 amount) const {
        return { min - Vec2f{ amount, amount }, max + Vec2f{ amount, amount } };
    }
    AABB2D Merged(const AABB2D& o) const {
        return { Vec2f{ Min(min.x, o.min.x), Min(min.y, o.min.y) },
                 Vec2f{ Max(max.x, o.max.x), Max(max.y, o.max.y) } };
    }
};

struct AABB {
    Vec3f min{  1e30f,  1e30f,  1e30f };
    Vec3f max{ -1e30f, -1e30f, -1e30f };

    Vec3f Center()   const { return (min + max) * 0.5f; }
    Vec3f HalfSize() const { return (max - min) * 0.5f; }
    bool  IsValid()  const { return min.x<=max.x && min.y<=max.y && min.z<=max.z; }

    bool Contains(Vec3f p) const {
        return p.x>=min.x && p.x<=max.x &&
               p.y>=min.y && p.y<=max.y &&
               p.z>=min.z && p.z<=max.z;
    }
    bool Overlaps(const AABB& o) const {
        return min.x<=o.max.x && max.x>=o.min.x &&
               min.y<=o.max.y && max.y>=o.min.y &&
               min.z<=o.max.z && max.z>=o.min.z;
    }
};

// ─────────────────────────────────────────────────────────────────────────────
//  Circle
// ─────────────────────────────────────────────────────────────────────────────
struct Circle {
    Vec2f center;
    f32   radius = 1.0f;

    bool Contains(Vec2f p) const {
        return (p - center).LengthSquared() <= radius * radius;
    }
    bool Overlaps(const Circle& o) const {
        f32 dist2 = (center - o.center).LengthSquared();
        f32 sum   = radius + o.radius;
        return dist2 <= sum * sum;
    }
    AABB2D Bounds() const {
        return { center - Vec2f{radius,radius}, center + Vec2f{radius,radius} };
    }
};

// ─────────────────────────────────────────────────────────────────────────────
//  Plane  (3-D half-space)
// ─────────────────────────────────────────────────────────────────────────────
struct Plane {
    Vec3f normal;       // must be unit
    f32   distance = 0; // signed distance from origin  (n·x = d for points on plane)

    static Plane FromNormalPoint(Vec3f n, Vec3f p) {
        return { n.Normalized(), n.Dot(p) };
    }

    f32 SignedDistance(Vec3f p) const { return normal.Dot(p) - distance; }
    bool IsInFront(Vec3f p)    const { return SignedDistance(p) >= 0.0f; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  Ray (3-D and 2-D)
// ─────────────────────────────────────────────────────────────────────────────
struct Ray {
    Vec3f origin;
    Vec3f direction; // unit vector

    Vec3f At(f32 t) const { return origin + direction * t; }
};

struct Ray2D {
    Vec2f origin;
    Vec2f direction; // unit vector

    Vec2f At(f32 t) const { return origin + direction * t; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  Frustum — 6 planes (near, far, left, right, top, bottom)
// ─────────────────────────────────────────────────────────────────────────────
struct Frustum {
    Plane planes[6]; // 0=near 1=far 2=left 3=right 4=bottom 5=top

    // Test if a sphere is at least partially inside the frustum
    bool IntersectsSphere(Vec3f center, f32 r) const {
        for (auto& p : planes) {
            if (p.SignedDistance(center) < -r) return false;
        }
        return true;
    }

    bool ContainsPoint(Vec3f p) const {
        for (auto& pl : planes) {
            if (!pl.IsInFront(p)) return false;
        }
        return true;
    }
};

// ─────────────────────────────────────────────────────────────────────────────
//  Triangle (3-D)
// ─────────────────────────────────────────────────────────────────────────────
struct Triangle {
    Vec3f a, b, c;

    Vec3f Normal() const { return (b - a).Cross(c - a).Normalized(); }
    Vec3f Centroid() const { return (a + b + c) * (1.0f / 3.0f); }
    f32   Area() const { return (b - a).Cross(c - a).Length() * 0.5f; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  Polygon (2-D convex or concave)
// ─────────────────────────────────────────────────────────────────────────────
struct Polygon {
    std::vector<Vec2f> vertices;

    Polygon() = default;
    explicit Polygon(std::initializer_list<Vec2f> verts) : vertices(verts) {}
    explicit Polygon(std::vector<Vec2f> verts) : vertices(std::move(verts)) {}

    usize VertexCount() const { return vertices.size(); }

    // Signed area (positive = CCW winding)
    f32 SignedArea() const {
        f32 area = 0;
        usize n = vertices.size();
        for (usize i = 0; i < n; ++i) {
            const Vec2f& a = vertices[i];
            const Vec2f& b = vertices[(i + 1) % n];
            area += a.x * b.y - b.x * a.y;
        }
        return area * 0.5f;
    }
    f32   Area()    const { return Abs(SignedArea()); }
    bool  IsCCW()   const { return SignedArea() > 0; }

    Vec2f Centroid() const {
        Vec2f c{ 0, 0 };
        for (auto& v : vertices) c += v;
        return (vertices.empty()) ? c : c / static_cast<f32>(vertices.size());
    }

    // Point-in-polygon — ray casting
    bool Contains(Vec2f p) const {
        bool inside = false;
        usize n = vertices.size();
        for (usize i = 0, j = n - 1; i < n; j = i++) {
            const Vec2f& vi = vertices[i];
            const Vec2f& vj = vertices[j];
            if (((vi.y > p.y) != (vj.y > p.y)) &&
                (p.x < (vj.x - vi.x) * (p.y - vi.y) / (vj.y - vi.y) + vi.x))
                inside = !inside;
        }
        return inside;
    }

    AABB2D Bounds() const { return AABB2D::FromPoints(vertices); }
};

// ─────────────────────────────────────────────────────────────────────────────
//  Intersection helpers
// ─────────────────────────────────────────────────────────────────────────────
struct RayHit2D {
    bool  hit      = false;
    f32   t        = 0;
    Vec2f point;
    Vec2f normal;
};

struct RayHit {
    bool  hit      = false;
    f32   t        = 0;
    Vec3f point;
    Vec3f normal;
};

// Ray vs AABB2D (slab test)
inline RayHit2D RayVsAABB2D(const Ray2D& ray, const AABB2D& box) {
    f32 tmin = -1e30f, tmax = 1e30f;
    const f32* bmin = &box.min.x;
    const f32* bmax = &box.max.x;
    const f32* ro   = &ray.origin.x;
    const f32* rd   = &ray.direction.x;

    for (int i = 0; i < 2; ++i) {
        if (Abs(rd[i]) < 1e-7f) {
            if (ro[i] < bmin[i] || ro[i] > bmax[i]) return {};
        } else {
            f32 t1 = (bmin[i] - ro[i]) / rd[i];
            f32 t2 = (bmax[i] - ro[i]) / rd[i];
            if (t1 > t2) { f32 tmp=t1; t1=t2; t2=tmp; }
            tmin = Max(tmin, t1);
            tmax = Min(tmax, t2);
            if (tmin > tmax) return {};
        }
    }
    if (tmax < 0) return {};
    f32 t = (tmin >= 0) ? tmin : tmax;
    return { true, t, ray.At(t), Vec2f{} };
}

// Ray vs Circle
inline RayHit2D RayVsCircle(const Ray2D& ray, const Circle& circle) {
    Vec2f oc = ray.origin - circle.center;
    f32 a = ray.direction.Dot(ray.direction);
    f32 b = 2.0f * oc.Dot(ray.direction);
    f32 c = oc.Dot(oc) - circle.radius * circle.radius;
    f32 disc = b*b - 4*a*c;
    if (disc < 0) return {};
    f32 t = (-b - Sqrt(disc)) / (2*a);
    if (t < 0) t = (-b + Sqrt(disc)) / (2*a);
    if (t < 0) return {};
    Vec2f pt = ray.At(t);
    Vec2f n  = (pt - circle.center).Normalized();
    return { true, t, pt, n };
}

} // namespace Shape::Math

// Pull key types into Shape namespace
namespace Shape {
    using Math::AABB;
    using Math::AABB2D;
    using Math::Circle;
    using Math::Plane;
    using Math::Ray;
    using Math::Ray2D;
    using Math::Frustum;
    using Math::Triangle;
    using Math::Polygon;
    using Math::RayHit;
    using Math::RayHit2D;
}
