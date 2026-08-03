#pragma once

/// @file ResourceComponents.hpp
/// @brief ECS2 components for food deposits and world resources.

#include "Core/Platform.hpp"
#include "Math/Vec2.hpp"

namespace Shape {

/// A food deposit in the world.
struct FoodDeposit {
    float energy      = 50.0f;  ///< Current energy content
    float max_energy  = 50.0f;  ///< Maximum energy content
    float regen_rate  = 0.2f;   ///< Energy regenerated per tick (at full sunlight)
    bool  depleted    = false;  ///< True when energy <= 0
    u8    _pad[3]{};
};

/// Marker component for the spatial grid to track food deposits separately.
struct FoodTag {};

/// World-level resource state (stored as WorldState extension or separate resource).
struct ResourceMapState {
    u32  food_deposit_count   = 0;
    u32  target_food_deposits = 500;  ///< Scheduler respawns food to reach this count
    float respawn_rate        = 0.05f; ///< New deposits per tick (probabilistic)
    float min_food_per_deposit = 10.0f;
    float max_food_per_deposit = 100.0f;
};

} // namespace Shape
