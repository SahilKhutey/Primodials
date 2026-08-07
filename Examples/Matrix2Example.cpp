#include "ShapeEngine/Math/Matrix2.hpp"
#include <iostream>

int main() {
    using namespace Shape::Math;
    using namespace Shape::Math::Literals;

    Matrix2f R = Matrix2f::Rotation(45.0_deg);
    Vector2 p(5.0f, 2.0f);

    Vector2 rotated = R * p;
    std::cout << "Rotated vector (45 deg): (" << rotated.x << ", " << rotated.y << ")" << std::endl;

    Matrix2f scale = Matrix2f::Scale(2.0f, 0.5f);
    Vector2 scaled = scale * p;
    std::cout << "Scaled vector: (" << scaled.x << ", " << scaled.y << ")" << std::endl;

    return 0;
}
