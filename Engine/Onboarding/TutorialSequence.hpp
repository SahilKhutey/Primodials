#pragma once

#include "Core/Platform.hpp"
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace Shape::Onboarding {

enum class TriggerType { Manual, Event, Time };
enum class HighlightType { None, Panel, WorldPosition, ScreenArea };

struct TutorialStep {
    std::string id;
    std::string title;
    std::string body;
    HighlightType highlight = HighlightType::None;
    std::string highlightTarget;
    TriggerType trigger = TriggerType::Manual;
    std::string triggerEvent;
    f32 triggerDelay = 0.0f;
};

class TutorialSequence {
public:
    TutorialSequence() = default;
    ~TutorialSequence() = default;

    void SetSteps(const std::vector<TutorialStep>& steps);
    void Start();
    void Stop();
    void Advance();
    void GoToStep(const std::string& stepId);
    void TriggerEvent(const std::string& eventName);
    void Update(f32 dt);

    bool IsActive() const { return m_active; }
    usize GetCurrentStepIndex() const { return m_currentStep; }
    usize GetTotalSteps() const { return m_steps.size(); }
    const TutorialStep* GetCurrentStep() const;

private:
    std::vector<TutorialStep> m_steps;
    usize m_currentStep = 0;
    bool m_active = false;
    f32 m_timer = 0.0f;
    std::vector<std::string> m_completedSteps;
    std::unordered_map<std::string, bool> m_pendingEvents;
};

} // namespace Shape::Onboarding
