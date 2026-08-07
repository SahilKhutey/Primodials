#include "ShapeEngine/Math/Quaternion.hpp"
#include "ShapeEngine/Math/Transform.hpp"
#include <iostream>

int main() {
    using namespace Shape::Math;
    using namespace Shape::Math::Literals;

    // Attitude quaternion — satellite facing along -Z (retrograde)
    Quaternionf attitude = Quaternionf::AxisAngle(Vector3f::UnitY(), 90.0_deg);

    Vector3f forward(0.0f, 0.0f, 1.0f);
    Vector3f worldForward = attitude.Rotate(forward);
    std::cout << "World forward: (" << worldForward.x << ", " << worldForward.y << ", " << worldForward.z << ")\n";

    // Chain two rotations
    Quaternionf tiltUp = Quaternionf::AxisAngle(Vector3f::UnitX(), -30.0_deg);
    Quaternionf combined = attitude * tiltUp;
    Vector3f tilted = combined.Rotate(forward);
    std::cout << "Tilted forward: (" << tilted.x << ", " << tilted.y << ", " << tilted.z << ")\n";

    // Transform a satellite position
    Transformf sat;
    sat.position = Vector3f(7000.0f, 0.0f, 0.0f);
    sat.rotation = Quaternionf::LookRotation(Vector3f::UnitZ(), Vector3f::UnitY());
    sat.scale    = Vector3f(1.0f, 1.0f, 1.0f);

    Vector3f sensor(0.0f, 0.0f, 10.0f);
    Vector3f sensorWorld = sat.TransformPoint(sensor);
    std::cout << "Sensor in world: (" << sensorWorld.x << ", " << sensorWorld.y << ", " << sensorWorld.z << ")\n";

    // Interpolate two attitudes
    Transformf a = Transformf::Identity();
    Transformf b;
    b.position = Vector3f(100.0f, 0.0f, 0.0f);
    b.rotation = Quaternionf::AxisAngle(Vector3f::UnitY(), 180.0_deg);

    Transformf mid = Transformf::Lerp(a, b, 0.5f);
    std::cout << "Lerped position: (" << mid.position.x << ", " << mid.position.y << ", " << mid.position.z << ")\n";

    return 0;
}
