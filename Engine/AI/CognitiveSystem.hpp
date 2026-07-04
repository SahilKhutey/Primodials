#pragma once

#include "ECS/World.hpp"

namespace Shape {

class CognitiveSystem {
public:
    CognitiveSystem() = default;
    ~CognitiveSystem() = default;

    void Update(World& world, f32 dt);
};

} // namespace Shape
