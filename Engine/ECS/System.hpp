#pragma once

#include "Core/Platform.hpp"
#include "ECS/UpdateContext.hpp"
#include <string_view>

namespace Shape {

class World;

// ─────────────────────────────────────────────────────────────────────────────
//  ExecutionPhase — determines the slot in which a System runs each tick.
//  Systems within the same phase run in the order they were registered.
// ─────────────────────────────────────────────────────────────────────────────
enum class ExecutionPhase : u8 {
    PreInput        = 0,   ///< Before input is processed
    Input           = 1,   ///< Input collection
    PreSimulation   = 2,   ///< Pre-tick bookkeeping
    Simulation      = 3,   ///< Core simulation — biology, physics, AI, etc.
    PostSimulation  = 4,   ///< Post-tick cleanup, event dispatch
    PreRender       = 5,   ///< Prepare render data (interpolation, culling)
    Render          = 6,   ///< Submit draw calls
    PostRender      = 7,   ///< Frame finalization
    LateUpdate      = 8,   ///< Cleanup, debug overlays

    COUNT           = 9
};

// ─────────────────────────────────────────────────────────────────────────────
//  System — base class for all game / engine systems.
//
//  Subclass, override Update(), register with SystemScheduler.
//  Systems are NOT automatically deleted by the scheduler; ownership lies with
//  the registering code.
// ─────────────────────────────────────────────────────────────────────────────
class System {
public:
    virtual ~System() = default;

    /// Called once after the system is registered with a World.
    virtual void OnRegister(World& /*world*/) {}

    /// Called once just before the system is unregistered.
    virtual void OnUnregister(World& /*world*/) {}

    /// Called every tick (or every frame for render-phase systems).
    virtual void Update(World& world, const UpdateContext& ctx) = 0;

    // ── Identification ────────────────────────────────────────────────────────
    virtual std::string_view Name()  const = 0;
    virtual ExecutionPhase   Phase() const = 0;

    /// Systems with lower priority numbers run first within the same phase.
    virtual i32 Priority() const { return 0; }

    bool IsEnabled() const { return m_Enabled; }
    void SetEnabled(bool enabled) { m_Enabled = enabled; }

private:
    bool m_Enabled = true;
};

// ─────────────────────────────────────────────────────────────────────────────
//  Convenience macro for declaring Name() and Phase() in one line.
//
//  Usage:
//    class MovementSystem : public System {
//        SHAPE_SYSTEM("MovementSystem", ExecutionPhase::Simulation);
//        void Update(World& world, const UpdateContext& ctx) override { ... }
//    };
// ─────────────────────────────────────────────────────────────────────────────
#define SHAPE_SYSTEM(name_str, phase_val)                               \
    std::string_view Name()  const override { return name_str; }        \
    ExecutionPhase   Phase() const override { return phase_val; }

} // namespace Shape
