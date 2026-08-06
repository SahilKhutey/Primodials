#include "Steam/Achievements/AchievementManager.hpp"
#include "Steam/SteamContext.hpp"

namespace Shape::Steam {

AchievementManager::AchievementManager(SteamContext* context)
    : m_context(context) {}

bool AchievementManager::Unlock(Achievement achievement) {
    if (m_unlocked[achievement]) return false;
    m_unlocked[achievement] = true;
    if (m_onUnlock) m_onUnlock(achievement);
    return true;
}

bool AchievementManager::IsUnlocked(Achievement achievement) const {
    auto it = m_unlocked.find(achievement);
    if (it != m_unlocked.end()) return it->second;
    return false;
}

f32 AchievementManager::GetProgress(Achievement achievement) const {
    auto it = m_progress.find(achievement);
    if (it != m_progress.end() && it->second.second > 0) {
        return static_cast<f32>(it->second.first) / static_cast<f32>(it->second.second);
    }
    return 0.0f;
}

bool AchievementManager::SetProgress(Achievement achievement, u32 current, u32 max) {
    m_progress[achievement] = { current, max };
    if (current >= max) {
        return Unlock(achievement);
    }
    return false;
}

std::string AchievementManager::GetName(Achievement achievement) const {
    auto idx = static_cast<size_t>(achievement);
    if (idx < static_cast<size_t>(Achievement::Count)) {
        return g_achievementDisplayNames[idx];
    }
    return "Unknown";
}

} // namespace Shape::Steam
