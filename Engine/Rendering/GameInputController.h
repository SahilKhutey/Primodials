// Engine/Rendering/GameInputController.h
#pragma once

#include "Rendering/CameraController.h"
#include "Shape/Simulation/SimulationController.h"
#include "ShapeEngine/Math/Vector2.hpp"

namespace Shape::Rendering {

enum class InputAction {
    None,
    PanLeft,
    PanRight,
    PanUp,
    PanDown,
    ZoomIn,
    ZoomOut,
    ResetCamera,
    TogglePause,
    StepSingleTick,
    SpeedUp,
    SlowDown
};

class GameInputController {
public:
    GameInputController() = default;
    GameInputController(CameraController* camera, Simulation::SimulationController* sim_controller);
    ~GameInputController() = default;

    void set_camera(CameraController* camera) { m_camera = camera; }
    void set_sim_controller(Simulation::SimulationController* sim_controller) { m_sim_controller = sim_controller; }

    // Execute input action
    void ExecuteAction(InputAction action, f32 scale = 1.0f);

    // Continuous pan update
    void Update(f32 dt, Math::Vector2f pan_input, f32 zoom_delta, const Math::Vector2f& screen_size);

    void SetPanSpeed(f32 speed) { m_pan_speed = speed; }
    f32 GetPanSpeed() const { return m_pan_speed; }

private:
    CameraController* m_camera = nullptr;
    Simulation::SimulationController* m_sim_controller = nullptr;
    f32 m_pan_speed = 300.0f; // Pixels / world units per second
};

} // namespace Shape::Rendering
