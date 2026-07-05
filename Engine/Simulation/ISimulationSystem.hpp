#pragma once

/// @file ISimulationSystem.hpp
/// @brief Interface all Phase-C simulation systems implement.
///        Registered with SimulationScheduler and called each tick.

#include "Core/Platform.hpp"
#include "Simulation/DeterministicRng.hpp"
#include <string_view>

namespace Shape {

// Forward declare ─ full definition in DeterministicRng.hpp
using DeterministicRng = Simulation::DeterministicRng;

/// Subset of world information passed to each simulation system per tick.
struct TickContext {
    u64    tick;         ///< Monotone tick counter (1-indexed)
    double fixed_delta;  ///< Fixed timestep in seconds (1.0 / tick_rate)
    double sim_time;     ///< Accumulated simulation time in seconds
    DeterministicRng& rng; ///< Shared deterministic RNG — use exclusively
};

/// Priority bands — lower number = earlier execution.
namespace SystemPriority {
    static constexpr int Spatial     = -100;
    static constexpr int PreSim      =  -50;
    static constexpr int Biology     =    0;
    static constexpr int Genetics    =   10;
    static constexpr int AI          =   20;
    static constexpr int Physics     =   30;
    static constexpr int Evolution   =  100;
    static constexpr int RenderSync  =  200;
}

/// Interface all simulation systems must implement.
class ISimulationSystem {
public:
    virtual ~ISimulationSystem() = default;

    virtual void on_register()   {}
    virtual void on_unregister() {}
    virtual void update(const TickContext& ctx) = 0;
    virtual int  priority() const = 0;
    virtual std::string_view name() const = 0;
};

} // namespace Shape
