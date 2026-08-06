#pragma once

#include "Steam/Achievements/AchievementDefinitions.hpp"
#include <unordered_map>
#include <vector>
#include <functional>
#include <string>

namespace Shape::Steam {

class SteamContext;

class AchievementManager {
public:
    explicit AchievementManager(SteamContext* context);
    ~AchievementManager() = default;

    bool Unlock(Achievement achievement);
    bool IsUnlocked(Achievement achievement) const;

    f32 GetProgress(Achievement achievement) const;
    bool SetProgress(Achievement achievement, u32 current, u32 max);

    std::string GetName(Achievement achievement) const;

    using UnlockCallback = std::function<void(Achievement)>;
    void SetOnUnlock(UnlockCallback cb) { m_onUnlock = std::move(cb); }

    SteamContext* GetContext() const { return m_context; }

private:
    SteamContext* m_context = nullptr;
    std::unordered_map<Achievement, bool> m_unlocked;
    std::unordered_map<Achievement, std::pair<u32, u32>> m_progress;
    UnlockCallback m_onUnlock;
};

} // namespace Shape::Steam
