#pragma once

#include "Core/Platform.hpp"
#include <unordered_map>
#include <string>

namespace Shape::Steam {

class SteamContext;

enum class Stat {
    TimePlayedSeconds,
    MaxPopulationAchieved,
    TotalEntitiesSpawned,
    TotalEntitiesDespawned,
    MaxGeneration,
    Count
};

class StatTracker {
public:
    explicit StatTracker(SteamContext* context);
    ~StatTracker() = default;

    i32 GetInt(Stat stat) const;
    void SetInt(Stat stat, i32 value);
    void AddInt(Stat stat, i32 delta);

    SteamContext* GetContext() const { return m_context; }

private:
    SteamContext* m_context = nullptr;
    std::unordered_map<Stat, i32> m_intStats;
};

} // namespace Shape::Steam
