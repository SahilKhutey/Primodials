// SteamStub.h — Offline Steam API stub for development
#pragma once

// This file provides a complete offline implementation of the Steamworks API
// for development without the Steam client. When STEAMWORKS_SDK is defined,
// the real SDK headers are used instead.

#ifdef STEAMWORKS_SDK
    #include "steam/steam_api.h"
    #include "steam/isteamuser.h"
    #include "steam/isteamfriends.h"
    #include "steam/isteamutils.h"
    #include "steam/isteamapps.h"
    #include "steam/isteamcloud.h"
    #include "steam/isteamremotestorage.h"
    #include "steam/isteamugc.h"
    #include "steam/isteamhtmlsurface.h"
    #include "steam/isteamhttp.h"
#else
    #include <cstdint>
    // Stub types
    using SteamAPICall_t = uint64_t;
    using HSteamUser = int32_t;
    using HSteamPipe = int32_t;
    constexpr HSteamUser k_steamUserDefaultInstance = 0;
    constexpr HSteamPipe k_steamPipeNone = -1;

    enum class EResult : int32_t {
        k_EResultOK = 1,
        k_EResultFail = 2,
        k_EResultNoConnection = 3,
        k_EResultInvalidParam = 8,
        k_EResultFileNotFound = 9,
        k_EResultLimitExceeded = 15
    };

    struct CSteamID {
        uint64_t id = 0;
        uint64_t ConvertToUint64() const { return id; }
    };

    class ISteamUser {
    public:
        virtual bool BLoggedOn() = 0;
        virtual CSteamID GetSteamID() = 0;
        virtual int GetPlayerSteamLevel() = 0;
    };

    class ISteamFriends {
    public:
        virtual const char* GetPersonaName() = 0;
        virtual void ActivateGameOverlay(const char* dialog) = 0;
    };

    class ISteamUtils {
    public:
        virtual uint32_t GetAppID() = 0;
        virtual const char* GetIPCountry() = 0;
        virtual bool IsOverlayEnabled() = 0;
    };

    class ISteamApps {
    public:
        virtual bool BIsSubscribed() = 0;
        virtual bool BIsLowViolence() = 0;
        virtual bool BIsCybercafe() = 0;
        virtual const char* GetCurrentGameLanguage() = 0;
        virtual bool BIsDlcInstalled(uint32_t appID) = 0;
    };

    class ISteamCloud {
    public:
        virtual bool IsCloudEnabledForAccount() = 0;
        virtual bool IsCloudEnabledForApp() = 0;
    };

    class ISteamRemoteStorage {
    public:
        virtual bool FileExists(const char* filename) = 0;
        virtual int32_t GetFileSize(const char* filename) = 0;
        virtual int32_t FileRead(const char* filename, void* data, int32_t size) = 0;
        virtual bool FileWrite(const char* filename, const void* data, int32_t size) = 0;
        virtual bool FileDelete(const char* filename) = 0;
        virtual int32_t GetQuota() = 0;
        virtual int32_t GetQuotaAvailable() = 0;
    };

    class ISteamUserStats {
    public:
        virtual bool RequestCurrentStats() = 0;
        virtual bool GetStat(const char* name, int32_t* pData) = 0;
        virtual bool GetStat(const char* name, float* pData) = 0;
        virtual bool SetStat(const char* name, int32_t data) = 0;
        virtual bool SetStat(const char* name, float data) = 0;
        virtual bool UpdateAvgRateStat(const char* name, float countThisSession, double sessionLength) = 0;
        virtual bool GetAchievement(const char* name, bool* pbAchieved) = 0;
        virtual bool SetAchievement(const char* name) = 0;
        virtual bool ClearAchievement(const char* name) = 0;
        virtual bool StoreStats() = 0;
        virtual bool IndicateAchievementProgress(const char* name, uint32_t currentProgress, uint32_t maxProgress) = 0;
    };

    // Global accessors
    inline bool SteamAPI_Init() { return true; }
    inline void SteamAPI_Shutdown() {}
    inline void SteamAPI_RunCallbacks() {}
    
    inline ISteamUser* SteamUser() { return nullptr; }
    inline ISteamFriends* SteamFriends() { return nullptr; }
    inline ISteamUtils* SteamUtils() { return nullptr; }
    inline ISteamApps* SteamApps() { return nullptr; }
    inline ISteamRemoteStorage* SteamRemoteStorage() { return nullptr; }
    inline ISteamUserStats* SteamUserStats() { return nullptr; }

#endif
