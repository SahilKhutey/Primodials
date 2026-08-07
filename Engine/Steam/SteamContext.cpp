#include "Steam/SteamContext.hpp"
#include "Steam/Achievements/AchievementManager.hpp"
#include "Steam/Workshop/WorkshopManager.hpp"
#include "Steam/Cloud/CloudSaveManager.hpp"
#include "Steam/Presence/RichPresence.hpp"
#include "Steam/Stats/StatTracker.hpp"

namespace Shape::Steam {

SteamContext::SteamContext() = default;

SteamContext::~SteamContext() {
    Shutdown();
}

bool SteamContext::Initialize(const SteamContextConfig& config) {
    m_config = config;
    m_initialized = true;
    return true;
}

void SteamContext::RunCallbacks() {}

void SteamContext::Shutdown() {
    m_achievements.reset();
    m_workshop.reset();
    m_cloud.reset();
    m_presence.reset();
    m_stats.reset();
    m_initialized = false;
}

AchievementManager* SteamContext::GetAchievementManager() {
    if (!m_achievements) m_achievements = std::make_unique<AchievementManager>(this);
    return m_achievements.get();
}

WorkshopManager* SteamContext::GetWorkshopManager() {
    if (!m_workshop) m_workshop = std::make_unique<WorkshopManager>(this);
    return m_workshop.get();
}

CloudSaveManager* SteamContext::GetCloudSaveManager() {
    if (!m_cloud) m_cloud = std::make_unique<CloudSaveManager>(this);
    return m_cloud.get();
}

RichPresence* SteamContext::GetRichPresence() {
    if (!m_presence) m_presence = std::make_unique<RichPresence>(this);
    return m_presence.get();
}

StatTracker* SteamContext::GetStatTracker() {
    if (!m_stats) m_stats = std::make_unique<StatTracker>(this);
    return m_stats.get();
}

} // namespace Shape::Steam
