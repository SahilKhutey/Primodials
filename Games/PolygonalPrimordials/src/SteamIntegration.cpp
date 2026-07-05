// SteamIntegration.cpp
#include "SteamIntegration.h"
#include "SteamStub.h"
#include "Shape/Core/Logger.h"

namespace PolygonalPrimordials::Steam {

    SteamIntegration& SteamIntegration::instance() {
        static SteamIntegration s;
        return s;
    }

    bool SteamIntegration::initialize() {
#ifdef STEAMWORKS_SDK
        if (!SteamAPI_Init()) {
            SHAPE_LOG_ERROR("Steam", "SteamAPI_Init failed");
            return false;
        }
        auto* user = SteamUser();
        auto* friends = SteamFriends();
        auto* utils = SteamUtils();
        auto* apps = SteamApps();

        if (user) {
            m_player.steam_id = user->GetSteamID().ConvertToUint64();
            m_player.online = user->BLoggedOn();
        }
        if (friends) {
            m_player.persona_name = friends->GetPersonaName();
        }
        if (utils) {
            m_player.country_code = utils->GetIPCountry();
        }
        if (apps) {
            m_player.language = apps->GetCurrentGameLanguage();
        }
        m_initialized = true;
        SHAPE_LOG_INFO("Steam", "Initialized for user: {} (ID: {})",
                       m_player.persona_name, m_player.steam_id);
#else
        // Stub mode: pretend we're a single-player dev build
        m_player.steam_id = 0xDEADBEEF;
        m_player.persona_name = "DevUser";
        m_player.country_code = "US";
        m_player.language = "english";
        m_player.online = true;
        m_player.level = 42;
        m_initialized = true;
        SHAPE_LOG_INFO("Steam", "Running in stub mode (no Steam client)");
#endif
        return true;
    }

    void SteamIntegration::shutdown() {
#ifdef STEAMWORKS_SDK
        SteamAPI_Shutdown();
#endif
        m_initialized = false;
    }

    void SteamIntegration::pump_callbacks() {
#ifdef STEAMWORKS_SDK
        SteamAPI_RunCallbacks();
#endif
    }

    bool SteamIntegration::is_online() const {
#ifdef STEAMWORKS_SDK
        return m_initialized && SteamUser() && SteamUser()->BLoggedOn();
#else
        return m_initialized;
#endif
    }

    bool SteamIntegration::is_subscribed() const {
#ifdef STEAMWORKS_SDK
        return SteamApps() && SteamApps()->BIsSubscribed();
#else
        return m_initialized;
#endif
    }

    void SteamIntegration::open_overlay(const std::string& dialog) {
#ifdef STEAMWORKS_SDK
        if (SteamFriends() && SteamUtils() && SteamUtils()->IsOverlayEnabled()) {
            SteamFriends()->ActivateGameOverlay(dialog.c_str());
        }
#endif
    }

    void SteamIntegration::set_stat(const std::string& name, int32_t value) {
#ifdef STEAMWORKS_SDK
        if (SteamUserStats()) SteamUserStats()->SetStat(name.c_str(), value);
#else
        SHAPE_LOG_DEBUG("Steam", "Stat {} = {} (stub)", name, value);
#endif
    }

    void SteamIntegration::set_stat(const std::string& name, float value) {
#ifdef STEAMWORKS_SDK
        if (SteamUserStats()) SteamUserStats()->SetStat(name.c_str(), value);
#else
        SHAPE_LOG_DEBUG("Steam", "Stat {} = {} (stub)", name, value);
#endif
    }

    int32_t SteamIntegration::get_stat_int(const std::string& name) const {
#ifdef STEAMWORKS_SDK
        int32_t v = 0;
        if (SteamUserStats()) SteamUserStats()->GetStat(name.c_str(), &v);
        return v;
#else
        (void)name;
        return 0;
#endif
    }

    float SteamIntegration::get_stat_float(const std::string& name) const {
#ifdef STEAMWORKS_SDK
        float v = 0;
        if (SteamUserStats()) SteamUserStats()->GetStat(name.c_str(), &v);
        return v;
#else
        (void)name;
        return 0;
#endif
    }

    bool SteamIntegration::store_stats() {
#ifdef STEAMWORKS_SDK
        if (SteamUserStats()) return SteamUserStats()->StoreStats();
        return false;
#else
        return true;
#endif
    }

    bool SteamIntegration::upload_leaderboard_score(const std::string& /*board*/, int32_t /*score*/, bool /*keep_best*/) {
#ifdef STEAMWORKS_SDK
        // Implementation
        return true;
#else
        return true;
#endif
    }

    std::vector<int32_t> SteamIntegration::download_leaderboard_entries(const std::string& /*board*/, int /*count*/) {
        return {};
    }

    bool SteamIntegration::is_dlc_installed(uint32_t app_id) const {
#ifdef STEAMWORKS_SDK
        return SteamApps() && SteamApps()->BIsDlcInstalled(app_id);
#else
        (void)app_id;
        return false;
#endif
    }

    void SteamIntegration::install_dlc(uint32_t /*app_id*/) {
#ifdef STEAMWORKS_SDK
        // SteamApps()->InstallDLC(app_id);
#endif
    }

} // namespace
