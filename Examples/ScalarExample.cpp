#include "ShapeEngine/Math/Scalar.hpp"
#include "Math/MathUtils.hpp"
#include <iostream>

int main() {
    using namespace Shape::Math;

    float clampedValue = Clamp(25.0f, 0.0f, 10.0f);
    std::cout << "Clamped value: " << clampedValue << " (expected: 10)" << std::endl;

    float lerpValue = Lerp(0.0f, 100.0f, 0.5f);
    std::cout << "Lerped value: " << lerpValue << " (expected: 50)" << std::endl;

    return 0;
}
