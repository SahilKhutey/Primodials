// Shape/Simulation/src/SimulationController.cpp
#include "Shape/Simulation/SimulationController.h"

namespace Shape::Simulation {

SimulationController::SimulationController()
    : m_state(SimulationState::Stopped) {}

SimulationController::SimulationController(SimulationClock* clock, SimulationScheduler* scheduler)
    : m_clock(clock), m_scheduler(scheduler), m_state(SimulationState::Stopped) {}

void SimulationController::Play() {
    m_state = SimulationState::Running;
    if (m_clock) {
        m_clock->SetPaused(false);
        m_clock->SetSpeedMultiplier(m_saved_time_scale > 0.0f ? m_saved_time_scale : 1.0f);
    }
    if (m_scheduler) {
        m_scheduler->resume();
        m_scheduler->set_speed(m_saved_time_scale > 0.0f ? m_saved_time_scale : 1.0f);
    }
}

void SimulationController::Pause() {
    m_state = SimulationState::Paused;
    if (m_clock) {
        m_clock->SetPaused(true);
    }
    if (m_scheduler) {
        m_scheduler->pause();
    }
}

void SimulationController::StepSingleTick() {
    m_state = SimulationState::StepSingleTick;
    if (m_scheduler) {
        m_scheduler->step_once();
        if (m_clock) {
            m_clock->SetTotalTicks(m_scheduler->current_tick());
        }
    } else if (m_clock) {
        m_clock->IncrementTicks();
    }
    m_state = SimulationState::Paused;
}

void SimulationController::FastForward(float time_scale) {
    m_state = SimulationState::FastForward;
    SetTimeScale(time_scale);
    if (m_clock) {
        m_clock->SetPaused(false);
    }
    if (m_scheduler) {
        m_scheduler->resume();
    }
}

void SimulationController::Stop() {
    m_state = SimulationState::Stopped;
    if (m_clock) {
        m_clock->SetPaused(true);
        m_clock->SetTotalTicks(0);
    }
    if (m_scheduler) {
        m_scheduler->pause();
    }
}

void SimulationController::SetTimeScale(float scale) {
    m_saved_time_scale = (scale > 0.0f) ? scale : 1.0f;
    if (m_clock) {
        m_clock->SetSpeedMultiplier(m_saved_time_scale);
    }
    if (m_scheduler) {
        m_scheduler->set_speed(m_saved_time_scale);
    }
}

float SimulationController::GetTimeScale() const noexcept {
    return m_clock ? m_clock->GetSpeedMultiplier() : m_saved_time_scale;
}

void SimulationController::SetSpeedPreset(uint32_t preset) {
    switch (preset) {
        case 1: SetTimeScale(1.0f); break;
        case 2: SetTimeScale(2.0f); break;
        case 3: SetTimeScale(5.0f); break;
        case 4: SetTimeScale(10.0f); break;
        default: SetTimeScale(1.0f); break;
    }
}

uint64_t SimulationController::GetCurrentTick() const noexcept {
    if (m_scheduler) return m_scheduler->current_tick();
    if (m_clock) return m_clock->GetTotalTicks();
    return 0;
}

void SimulationController::Update(uint64_t real_delta_us) {
    if (m_state == SimulationState::Paused || m_state == SimulationState::Stopped) {
        return;
    }

    if (m_scheduler) {
        double seconds = static_cast<double>(real_delta_us) / 1000000.0;
        m_scheduler->step(seconds);
        if (m_clock) {
            m_clock->SetTotalTicks(m_scheduler->current_tick());
        }
    }
}

} // namespace Shape::Simulation
