#include <catch2/catch_test_macros.hpp>
#include "Steam/SteamContext.hpp"
#include "Steam/Achievements/AchievementManager.hpp"
#include "Steam/Workshop/WorkshopManager.hpp"
#include "Steam/Cloud/CloudSaveManager.hpp"
#include "Steam/Presence/RichPresence.hpp"
#include "Steam/Stats/StatTracker.hpp"
#include "Distribution/BuildCooker/AssetCooker.hpp"
#include "Distribution/SteamApp/SteamAppConfig.hpp"

TEST_CASE("Steam Integration and Build Distribution Subsystem", "[Steam]") {
    SECTION("SteamAppConfig constants verification") {
        REQUIRE(Shape::Steam::SteamAppConfig::APP_ID == 480);
        REQUIRE(Shape::Steam::SteamAppConfig::ACHIEVEMENT_COUNT == 20);
        REQUIRE(std::string(Shape::Steam::SteamAppConfig::APP_NAME) == "Polygonal Primordials");
    }

    SECTION("SteamContext initialization and sub-manager creation") {
        Shape::Steam::SteamContext steam;
        Shape::Steam::SteamContextConfig config;
        config.offlineMode = true;

        REQUIRE(steam.Initialize(config));
        REQUIRE(steam.IsInitialized());
        REQUIRE_FALSE(steam.IsOnline());

        auto* achievements = steam.GetAchievementManager();
        REQUIRE(achievements != nullptr);

        auto* workshop = steam.GetWorkshopManager();
        REQUIRE(workshop != nullptr);

        auto* cloud = steam.GetCloudSaveManager();
        REQUIRE(cloud != nullptr);

        auto* presence = steam.GetRichPresence();
        REQUIRE(presence != nullptr);

        auto* stats = steam.GetStatTracker();
        REQUIRE(stats != nullptr);
    }

    SECTION("AchievementManager unlock and progress tracking") {
        Shape::Steam::SteamContext steam;
        steam.Initialize();

        auto* achievements = steam.GetAchievementManager();
        REQUIRE_FALSE(achievements->IsUnlocked(Shape::Steam::Achievement::FIRST_SPAWN));

        bool callbackFired = false;
        achievements->SetOnUnlock([&callbackFired](Shape::Steam::Achievement ach) {
            if (ach == Shape::Steam::Achievement::FIRST_SPAWN) {
                callbackFired = true;
            }
        });

        REQUIRE(achievements->Unlock(Shape::Steam::Achievement::FIRST_SPAWN));
        REQUIRE(achievements->IsUnlocked(Shape::Steam::Achievement::FIRST_SPAWN));
        REQUIRE(callbackFired);

        // Incremental progress unlock
        REQUIRE(achievements->SetProgress(Shape::Steam::Achievement::SPEED_DEMON, 10, 10));
        REQUIRE(achievements->IsUnlocked(Shape::Steam::Achievement::SPEED_DEMON));
    }

    SECTION("WorkshopManager item subscription and installation tracking") {
        Shape::Steam::SteamContext steam;
        steam.Initialize();

        auto* workshop = steam.GetWorkshopManager();
        u64 modId = 123456789ULL;

        REQUIRE_FALSE(workshop->IsInstalled(modId));
        REQUIRE(workshop->Subscribe(modId));
        REQUIRE(workshop->IsInstalled(modId));
        REQUIRE(workshop->Unsubscribe(modId));
        REQUIRE_FALSE(workshop->IsInstalled(modId));
    }

    SECTION("CloudSaveManager upload, download, and file listing") {
        Shape::Steam::SteamContext steam;
        steam.Initialize();

        auto* cloud = steam.GetCloudSaveManager();
        std::string filename = "quicksave.sim";
        std::vector<u8> saveData = { 'S', 'H', 'A', 'P', 'E', 'S', 'I', 'M' };

        REQUIRE(cloud->Upload(filename, saveData));

        std::vector<u8> downloadedData;
        REQUIRE(cloud->Download(filename, downloadedData));
        REQUIRE(downloadedData == saveData);

        auto fileList = cloud->ListFiles();
        REQUIRE(fileList.size() == 1);
        REQUIRE(fileList[0].name == filename);

        REQUIRE(cloud->DeleteFile(filename));
        REQUIRE(cloud->ListFiles().empty());
    }

    SECTION("RichPresence formatting and StatTracker counters") {
        Shape::Steam::SteamContext steam;
        steam.Initialize();

        auto* presence = steam.GetRichPresence();
        presence->SetWatching(5000);
        REQUIRE(presence->GetCurrentDisplay() == "watching#5000");

        auto* stats = steam.GetStatTracker();
        REQUIRE(stats->GetInt(Shape::Steam::Stat::TotalEntitiesSpawned) == 0);
        stats->AddInt(Shape::Steam::Stat::TotalEntitiesSpawned, 100);
        REQUIRE(stats->GetInt(Shape::Steam::Stat::TotalEntitiesSpawned) == 100);
    }

    SECTION("AssetCooker SHA256 integrity hashing and manifest creation") {
        Shape::AssetCooker cooker;
        std::vector<u8> sampleData = { 0x01, 0x02, 0x03, 0x04 };
        std::string hash = cooker.ComputeSHA256(sampleData);
        REQUIRE(!hash.empty());
        REQUIRE(hash.length() == 16);

        Shape::CookOptions opts;
        REQUIRE(cooker.CookAll(opts));
        auto manifest = cooker.GenerateManifest("Content");
        REQUIRE(!manifest.empty());
    }
}
