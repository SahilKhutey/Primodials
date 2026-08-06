#pragma once

#include "UI/IPanel.hpp"
#include <vector>

namespace Shape::UI {

class StatsPanel final : public IPanel {
public:
    StatsPanel();
    ~StatsPanel() override = default;

    void Draw(World& world) override;
    const char* GetName() const override { return "Engine Stats"; }

    void PushFrameTime(f32 frameTimeSeconds);
    f32 GetAverageFPS() const;
    f32 GetLastFrameTimeMs() const;

private:
    std::vector<f32> m_frameTimeHistory;
    usize m_historyIndex = 0;
    static constexpr usize HISTORY_CAPACITY = 120;
};

} // namespace Shape::UI
