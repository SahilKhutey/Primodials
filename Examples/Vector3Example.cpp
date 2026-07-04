#include "ShapeEngine/Math/Vector3.hpp"
#include <iostream>

int main() {
    using namespace Shape::Math;

    Vector3f position(5.0f, 4.0f, 2.0f);
    Vector3f velocity(1.0f, 0.0f, 0.0f);
    Vector3f normal(0.0f, 1.0f, 0.0f);

    Vector3f reflected = Reflect(velocity, normal);
    std::cout << "Reflected velocity: (" << reflected.x << ", " << reflected.y << ", " << reflected.z << ")" << std::endl;

    Vector3f direction = Normalize(position);
    Vector3f right = Cross(direction, Vector3f::Up());
    std::cout << "Computed right vector: (" << right.x << ", " << right.y << ", " << right.z << ")" << std::endl;

    return 0;
}
