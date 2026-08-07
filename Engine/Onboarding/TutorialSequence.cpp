#include "Onboarding/TutorialSequence.hpp"

namespace Shape::Onboarding {

void TutorialSequence::SetSteps(const std::vector<TutorialStep>& steps) {
    m_steps = steps;
    m_currentStep = 0;
}

void TutorialSequence::Start() {
    m_active = !m_steps.empty();
    m_currentStep = 0;
    m_timer = 0.0f;
}

void TutorialSequence::Stop() {
    m_active = false;
}

void TutorialSequence::Advance() {
    if (!m_active || m_currentStep >= m_steps.size()) return;

    m_completedSteps.push_back(m_steps[m_currentStep].id);
    m_currentStep++;
    m_timer = 0.0f;

    if (m_currentStep >= m_steps.size()) {
        m_active = false;
    }
}

void TutorialSequence::GoToStep(const std::string& stepId) {
    for (usize i = 0; i < m_steps.size(); ++i) {
        if (m_steps[i].id == stepId) {
            m_currentStep = i;
            m_timer = 0.0f;
            m_active = true;
            return;
        }
    }
}

void TutorialSequence::TriggerEvent(const std::string& eventName) {
    m_pendingEvents[eventName] = true;
}

void TutorialSequence::Update(f32 dt) {
    if (!m_active || m_currentStep >= m_steps.size()) return;

    const auto& step = m_steps[m_currentStep];

    if (step.trigger == TriggerType::Time) {
        m_timer += dt;
        if (m_timer >= step.triggerDelay) {
            Advance();
        }
    } else if (step.trigger == TriggerType::Event) {
        auto it = m_pendingEvents.find(step.triggerEvent);
        if (it != m_pendingEvents.end() && it->second) {
            m_pendingEvents.erase(it);
            Advance();
        }
    }
}

const TutorialStep* TutorialSequence::GetCurrentStep() const {
    if (m_active && m_currentStep < m_steps.size()) {
        return &m_steps[m_currentStep];
    }
    return nullptr;
}

} // namespace Shape::Onboarding
