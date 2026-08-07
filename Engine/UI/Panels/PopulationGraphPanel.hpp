#pragma once

#include "UI/IPanel.hpp"
#include <vector>

namespace Shape::UI {

struct PopulationDataPoint {
    f32 timestamp = 0.0f;
    usize preyCount = 0;
    usize predatorCount = 0;
    usize totalCount = 0;
};

class PopulationGraphPanel final : public IPanel {
public:
    PopulationGraphPanel();
    ~PopulationGraphPanel() override = default;

    void Draw(World& world) override;
    const char* GetName() const override { return "Population Dynamics"; }

    void PushSample(f32 timestamp, usize preyCount, usize predatorCount);
    const std::vector<PopulationDataPoint>& GetHistory() const { return m_history; }

private:
    std::vector<PopulationDataPoint> m_history;
    usize m_sampleIndex = 0;
    static constexpr usize HISTORY_CAPACITY = 60;
};

} // namespace Shape::UI
