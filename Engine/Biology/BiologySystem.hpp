#pragma once

#include "ECS/World.hpp"
#include "ECS/CommandBuffer.hpp"

namespace Shape {

class BiologySystem {
public:
    BiologySystem() = default;
    ~BiologySystem() = default;

    void Update(World& world, CommandBuffer& cmd, f32 dt, u32 frameSeed = 1337);

private:
    void ProcessLifecycles(World& world, CommandBuffer& cmd, f32 dt);
    void ProcessReproduction(World& world, CommandBuffer& cmd, u32 seed);
};

} // namespace Shape
