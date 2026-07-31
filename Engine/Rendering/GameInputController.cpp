// Engine/Rendering/GameInputController.cpp
#include "Rendering/GameInputController.h"

namespace Shape::Rendering {

GameInputController::GameInputController(CameraController* camera, Simulation::SimulationController* sim_controller)
    : m_camera(camera), m_sim_controller(sim_controller) {}

void GameInputController::ExecuteAction(InputAction action, f32 scale) {
    (void)scale;
    switch (action) {
        case InputAction::PanLeft:
            if (m_camera) m_camera->Pan({-50.0f, 0.0f});
            break;
        case InputAction::PanRight:
            if (m_camera) m_camera->Pan({50.0f, 0.0f});
            break;
        case InputAction::PanUp:
            if (m_camera) m_camera->Pan({0.0f, -50.0f});
            break;
        case InputAction::PanDown:
            if (m_camera) m_camera->Pan({0.0f, 50.0f});
            break;
        case InputAction::ZoomIn:
            if (m_camera) m_camera->SetZoom(m_camera->GetZoom() * 1.25f);
            break;
        case InputAction::ZoomOut:
            if (m_camera) m_camera->SetZoom(m_camera->GetZoom() * 0.8f);
            break;
        case InputAction::ResetCamera:
            if (m_camera) {
                m_camera->SetPosition({0.0f, 0.0f});
                m_camera->SetZoom(1.0f);
            }
            break;
        case InputAction::TogglePause:
            if (m_sim_controller) {
                if (m_sim_controller->IsPaused()) m_sim_controller->Play();
                else m_sim_controller->Pause();
            }
            break;
        case InputAction::StepSingleTick:
            if (m_sim_controller) m_sim_controller->StepSingleTick();
            break;
        case InputAction::SpeedUp:
            if (m_sim_controller) m_sim_controller->SetTimeScale(m_sim_controller->GetTimeScale() * 2.0f);
            break;
        case InputAction::SlowDown:
            if (m_sim_controller) m_sim_controller->SetTimeScale(m_sim_controller->GetTimeScale() * 0.5f);
            break;
        case InputAction::None:
            break;
    }
}

void GameInputController::Update(f32 dt, Math::Vector2f pan_input, f32 zoom_delta, const Math::Vector2f& screen_size) {
    if (m_camera) {
        if (pan_input.LengthSquared() > 0.001f) {
            Math::Vector2f delta_world = (pan_input * m_pan_speed * dt) / m_camera->GetZoom();
            m_camera->Pan(delta_world);
        }

        if (std::abs(zoom_delta) > 0.001f) {
            f32 zoom_factor = (zoom_delta > 0.0f) ? 1.15f : 0.85f;
            Math::Vector2f center = screen_size * 0.5f;
            m_camera->Zoom(zoom_factor, center, screen_size);
        }
    }
}

} // namespace Shape::Rendering
