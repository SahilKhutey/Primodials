#include "ShapeEngine/Math/Geometry/Point/PointAlgorithms.hpp"
#include <iostream>

int main() {
    using namespace Shape::Math;
    using namespace Shape::Math::Geometry;

    // 1. Basic displacement
    std::cout << "=== Point3 — Displacement ===\n";
    Point3f satellite(7000.0f, 0.0f, 0.0f);    // Position in km (ECEF approx)
    Vector3f delta(0.0f, 0.5f, 0.0f);           // Delta-V direction
    Point3f newPos = satellite + delta;
    std::cout << "  New position: (" << newPos.x << ", " << newPos.y << ", " << newPos.z << ")\n";

    // 2. Displacement vector between two points
    std::cout << "\n=== Point3 — Displacement Vector ===\n";
    Point3f target(7001.0f, 2.0f, 0.5f);
    Vector3f approach = target - satellite;
    std::cout << "  Approach vector: (" << approach.x << ", " << approach.y << ", " << approach.z << ")\n";
    float approachDist = Length(approach);
    std::cout << "  Approach dist: " << approachDist << " km\n";

    // 3. Centroid of a formation
    std::cout << "\n=== Formation Centroid ===\n";
    Point3f formation[4] = {
        {7000.0f, -1.0f, 0.0f},
        {7000.0f,  1.0f, 0.0f},
        {6999.0f,  0.0f, 1.0f},
        {7001.0f,  0.0f, -1.0f}
    };
    Point3f centroid = Centroid(formation, 4);
    std::cout << "  Centroid: (" << centroid.x << ", " << centroid.y << ", " << centroid.z << ")\n";

    // 4. Bounding box of formation
    std::cout << "\n=== Formation Bounding Box ===\n";
    auto bbox = BoundingBox(formation, 4);
    std::cout << "  Min: (" << bbox.minPt.x << ", " << bbox.minPt.y << ", " << bbox.minPt.z << ")\n";
    std::cout << "  Max: (" << bbox.maxPt.x << ", " << bbox.maxPt.y << ", " << bbox.maxPt.z << ")\n";

    // 5. Ritter bounding sphere of formation
    std::cout << "\n=== Formation Bounding Sphere (Ritter) ===\n";
    auto sphere = RitterSphere(formation, 4);
    std::cout << "  Center: (" << sphere.center.x << ", " << sphere.center.y << ", " << sphere.center.z << ")\n";
    std::cout << "  Radius: " << sphere.radius << " km\n";

    // 6. Homogeneous point perspective divide
    std::cout << "\n=== Point4 — Perspective Divide ===\n";
    Point4f screenPos(200.0f, 150.0f, 10.0f, 5.0f);
    Point3f ndc = screenPos.PerspectiveDivide();
    std::cout << "  NDC: (" << ndc.x << ", " << ndc.y << ", " << ndc.z << ")\n";

    // 7. Circumcircle (navigation beacon placement)
    std::cout << "\n=== Circumcircle of 3 Beacons ===\n";
    Point2f b1(0, 0), b2(10, 0), b3(5, 8);
    auto [cc, r, valid] = Circumcircle(b1, b2, b3);
    if (valid) {
        std::cout << "  Center: (" << cc.x << ", " << cc.y << "), Radius: " << r << "\n";
    }

    return 0;
}
