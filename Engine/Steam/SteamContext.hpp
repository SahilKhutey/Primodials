#pragma once

#include "Core/Platform.hpp"
#include <string>
#include <memory>

namespace Shape::Steam {

class AchievementManager;
class WorkshopManager;
class CloudSaveManager;
class RichPresence;
class StatTracker;

struct SteamContextConfig {
    u32 appId = 480;
    bool enableOverlay = true;
    bool offlineMode = true; // Default to offline mode unless explicitly initialized
    std::string language = "english";
};

class SteamContext {
public:
    SteamContext();
    ~SteamContext();

    bool Initialize(const SteamContextConfig& config = {});
    void RunCallbacks();
    void Shutdown();

    bool IsInitialized() const { return m_initialized; }
    bool IsOnline() const { return m_initialized && !m_config.offlineMode; }
    u64 GetLocalUserId() const { return m_localUserId; }
    std::string GetLocalUserName() const { return m_localUserName; }

    bool IsSteamRunningOnSteamDeck() const { return m_isSteamDeck; }
    std::string GetCurrentGameLanguage() const { return m_config.language; }

    AchievementManager* GetAchievementManager();
    WorkshopManager* GetWorkshopManager();
    CloudSaveManager* GetCloudSaveManager();
    RichPresence* GetRichPresence();
    StatTracker* GetStatTracker();

private:
    SteamContextConfig m_config;
    bool m_initialized = false;
    u64 m_localUserId = 76561198000000000ULL;
    std::string m_localUserName = "OfflineUser";
    bool m_isSteamDeck = false;

    std::unique_ptr<AchievementManager> m_achievements;
    std::unique_ptr<WorkshopManager> m_workshop;
    std::unique_ptr<CloudSaveManager> m_cloud;
    std::unique_ptr<RichPresence> m_presence;
    std::unique_ptr<StatTracker> m_stats;
};

} // namespace Shape::Steam
