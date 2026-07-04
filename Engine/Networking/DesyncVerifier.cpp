#include "DesyncVerifier.hpp"
#include "Utility/CRC32.hpp"
#include "ECS/TransformComponent.hpp"
#include <algorithm>

namespace Shape {

u32 DesyncVerifier::CalculateStateHash(World& world) {
    auto entities = world.Query<TransformComponent>();
    
    // Sort entities by ID to ensure deterministic iteration order!
    std::sort(entities.begin(), entities.end());

    u32 hash = 0xFFFFFFFF;
    
    for (Entity e : entities) {
        const auto& t = world.GetComponent<TransformComponent>(e);
        // Only hash critical positional data to avoid floating point inconsistencies on rotations
        hash = Utility::CRC32::Calculate(&t.position, sizeof(t.position), hash);
    }
    
    return hash;
}

} // namespace Shape
