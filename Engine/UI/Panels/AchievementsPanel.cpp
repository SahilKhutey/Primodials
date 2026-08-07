#include "UI/Panels/AchievementsPanel.hpp"

namespace Shape {

AchievementsPanel::AchievementsPanel(Steam::AchievementManager* manager)
    : m_manager(manager) {
    Refresh();
}

void AchievementsPanel::Refresh() {
    m_entries.clear();
    m_unlockedCount = 0;
    if (!m_manager) return;

    for (u32 i = 0; i < static_cast<u32>(Steam::Achievement::Count); ++i) {
        auto ach = static_cast<Steam::Achievement>(i);
        AchievementEntry entry;
        entry.id = ach;
        entry.name = m_manager->GetName(ach);
        entry.unlocked = m_manager->IsUnlocked(ach);
        entry.progress = m_manager->GetProgress(ach);
        entry.globalRarity = 12.5f;

        if (entry.unlocked) m_unlockedCount++;
        m_entries.push_back(entry);
    }

    m_completionPercent = m_entries.empty() ? 0.0f : (static_cast<f32>(m_unlockedCount) / m_entries.size()) * 100.0f;
}

void AchievementsPanel::Draw(World& world) {
    (void)world;
}

} // namespace Shape
