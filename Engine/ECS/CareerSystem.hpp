#pragma once

#include "ECS/World.hpp"

namespace Shape {

class CareerSystem {
public:
    CareerSystem() = default;
    ~CareerSystem() = default;

    void Update(World& world, f32 dt);
};

} // namespace Shape
