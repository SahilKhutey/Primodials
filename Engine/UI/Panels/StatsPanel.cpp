#include "UI/Panels/StatsPanel.hpp"
#include <numeric>
#include <algorithm>

namespace Shape::UI {

StatsPanel::StatsPanel() {
    m_frameTimeHistory.resize(HISTORY_CAPACITY, 1.0f / 60.0f);
}

void StatsPanel::PushFrameTime(f32 frameTimeSeconds) {
    if (frameTimeSeconds <= 0.00001f) frameTimeSeconds = 0.00001f;
    m_frameTimeHistory[m_historyIndex] = frameTimeSeconds;
    m_historyIndex = (m_historyIndex + 1) % HISTORY_CAPACITY;
}

f32 StatsPanel::GetAverageFPS() const {
    f32 sum = 0.0f;
    for (f32 t : m_frameTimeHistory) sum += t;
    f32 avgDt = sum / static_cast<f32>(HISTORY_CAPACITY);
    return avgDt > 0.00001f ? (1.0f / avgDt) : 0.0f;
}

f32 StatsPanel::GetLastFrameTimeMs() const {
    usize prevIdx = (m_historyIndex + HISTORY_CAPACITY - 1) % HISTORY_CAPACITY;
    return m_frameTimeHistory[prevIdx] * 1000.0f;
}

void StatsPanel::Draw(World& world) {
    // Stats calculation without GUI dependency for headless/test safety
    (void)world;
}

} // namespace Shape::UI
