#include "Steam/Stats/StatTracker.hpp"
#include "Steam/SteamContext.hpp"

namespace Shape::Steam {

StatTracker::StatTracker(SteamContext* context)
    : m_context(context) {}

i32 StatTracker::GetInt(Stat stat) const {
    auto it = m_intStats.find(stat);
    if (it != m_intStats.end()) return it->second;
    return 0;
}

void StatTracker::SetInt(Stat stat, i32 value) {
    m_intStats[stat] = value;
}

void StatTracker::AddInt(Stat stat, i32 delta) {
    SetInt(stat, GetInt(stat) + delta);
}

} // namespace Shape::Steam
