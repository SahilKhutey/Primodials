#pragma once

#include "UI/IPanel.hpp"
#include "Simulation/ReplaySystem.hpp"

namespace Shape::UI {

class ReplayPanel final : public IPanel {
public:
    explicit ReplayPanel(Simulation::ReplaySystem* replaySystem);
    ~ReplayPanel() override = default;

    void Draw(World& world) override;
    const char* GetName() const override { return "Replay Scrubbing"; }

private:
    Simulation::ReplaySystem* m_replaySystem = nullptr;
};

} // namespace Shape::UI
