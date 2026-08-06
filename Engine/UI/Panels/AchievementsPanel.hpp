#pragma once

#include "UI/IPanel.hpp"
#include "Steam/Achievements/AchievementDefinitions.hpp"
#include "Steam/Achievements/AchievementManager.hpp"
#include <vector>
#include <string>

namespace Shape {

struct AchievementEntry {
    Steam::Achievement id;
    std::string name;
    bool unlocked = false;
    f32 progress = 0.0f;
    f32 globalRarity = 0.0f;
};

class AchievementsPanel final : public UI::IPanel {
public:
    explicit AchievementsPanel(Steam::AchievementManager* manager);
    ~AchievementsPanel() override = default;

    void Draw(World& world) override;
    const char* GetName() const override { return "Achievements"; }
    void Refresh();

    const std::vector<AchievementEntry>& GetEntries() const { return m_entries; }
    usize GetUnlockedCount() const { return m_unlockedCount; }

private:
    Steam::AchievementManager* m_manager = nullptr;
    std::vector<AchievementEntry> m_entries;
    usize m_unlockedCount = 0;
    f32 m_completionPercent = 0.0f;
};

} // namespace Shape
