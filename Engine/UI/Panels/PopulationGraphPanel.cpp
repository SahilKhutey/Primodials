#include "UI/Panels/PopulationGraphPanel.hpp"
#include "ECS/TransformComponent.hpp"

namespace Shape::UI {

PopulationGraphPanel::PopulationGraphPanel() {
    m_history.resize(HISTORY_CAPACITY);
}

void PopulationGraphPanel::PushSample(f32 timestamp, usize preyCount, usize predatorCount) {
    PopulationDataPoint pt;
    pt.timestamp = timestamp;
    pt.preyCount = preyCount;
    pt.predatorCount = predatorCount;
    pt.totalCount = preyCount + predatorCount;

    m_history[m_sampleIndex] = pt;
    m_sampleIndex = (m_sampleIndex + 1) % HISTORY_CAPACITY;
}

void PopulationGraphPanel::Draw(World& world) {
    // Sample active world population count
    auto entities = world.Query<TransformComponent>();
    PushSample(0.0f, entities.size(), 0);
}

} // namespace Shape::UI
