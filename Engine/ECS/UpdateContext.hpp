#pragma once

#include "Core/Platform.hpp"
#include "Simulation/DeterministicRng.hpp"

namespace Shape {

class CommandBuffer;

// ─────────────────────────────────────────────────────────────────────────────
//  UpdateContext — passed to every System::Update() call.
//  Provides the tick timestamp, fixed timestep, and engine services.
// ─────────────────────────────────────────────────────────────────────────────
struct UpdateContext {
    u64               tick        = 0;       ///< Current simulation tick (monotonically increasing)
    f32               delta_time  = 0.0f;   ///< Fixed timestep in seconds (e.g., 1/60)
    f32               alpha       = 0.0f;   ///< Render interpolation factor [0,1)
    DeterministicRng* rng         = nullptr; ///< Per-frame RNG (owned by SimulationScheduler)
    CommandBuffer*    commands    = nullptr; ///< Deferred structural changes
};

} // namespace Shape
