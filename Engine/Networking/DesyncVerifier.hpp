#pragma once
#include "ECS/World.hpp"
#include "Core/Platform.hpp"

namespace Shape {

class DesyncVerifier {
public:
    u32 CalculateStateHash(World& world);
};

} // namespace Shape
