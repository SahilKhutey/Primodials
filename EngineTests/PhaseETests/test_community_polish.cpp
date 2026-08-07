#include <catch2/catch_test_macros.hpp>
#include "UI/Components/Toast.hpp"
#include "Onboarding/TutorialSequence.hpp"
#include "Community/LeaderboardClient.hpp"
#include "Community/FriendsListManager.hpp"
#include "Community/ScreenshotUploader.hpp"
#include "Steam/SteamContext.hpp"
#include "UI/Panels/AchievementsPanel.hpp"
#include "UI/Panels/WorkshopBrowserPanel.hpp"
#include "UI/Panels/CommunityHubPanel.hpp"

TEST_CASE("Community Polish Subsystem", "[Community]") {
    SECTION("ToastManager notification dispatch and auto-dismissal") {
        auto& toasts = Shape::UI::ToastManager::Get();
        toasts.Clear();

        u64 id1 = toasts.Show("Info", "Welcome to ShapeEngine!", Shape::UI::ToastLevel::Info, 2.0f);
        toasts.ShowAchievementUnlock("First Breath", "Spawn your first creature.");

        REQUIRE(toasts.GetActiveToasts().size() == 2);

        toasts.Update(1.0f);
        REQUIRE(toasts.GetActiveToasts().size() == 2);

        toasts.Update(1.5f);
        REQUIRE(toasts.GetActiveToasts().size() == 1);

        toasts.Dismiss(id1);
        toasts.Clear();
        REQUIRE(toasts.GetActiveToasts().empty());
    }

    SECTION("TutorialSequence step initialization and progression") {
        Shape::Onboarding::TutorialSequence tutorial;
        std::vector<Shape::Onboarding::TutorialStep> steps;

        Shape::Onboarding::TutorialStep s1;
        s1.id = "step1";
        s1.title = "Step 1";
        s1.trigger = Shape::Onboarding::TriggerType::Manual;
        steps.push_back(s1);

        Shape::Onboarding::TutorialStep s2;
        s2.id = "step2";
        s2.title = "Step 2";
        s2.trigger = Shape::Onboarding::TriggerType::Time;
        s2.triggerDelay = 1.0f;
        steps.push_back(s2);

        tutorial.SetSteps(steps);
        REQUIRE_FALSE(tutorial.IsActive());

        tutorial.Start();
        REQUIRE(tutorial.IsActive());
        REQUIRE(tutorial.GetCurrentStepIndex() == 0);
        REQUIRE(tutorial.GetCurrentStep()->id == "step1");

        tutorial.Advance();
        REQUIRE(tutorial.GetCurrentStepIndex() == 1);
        REQUIRE(tutorial.GetCurrentStep()->id == "step2");

        tutorial.Update(1.5f);
        REQUIRE_FALSE(tutorial.IsActive());
    }

    SECTION("LeaderboardClient score submission and caching") {
        Shape::Steam::SteamContext steam;
        steam.Initialize();

        Shape::Community::LeaderboardClient leaderboards(&steam);
        REQUIRE(leaderboards.SubmitScore(Shape::Community::Board::LongestLineage, 500));

        auto entries = leaderboards.GetEntries(Shape::Community::Board::LongestLineage);
        REQUIRE(entries.size() == 1);
        REQUIRE(entries[0].score == 500);
        REQUIRE(entries[0].isLocalPlayer);
    }

    SECTION("FriendsListManager status parsing and invitations") {
        Shape::Steam::SteamContext steam;
        steam.Initialize();

        Shape::Community::FriendsListManager friends(&steam);
        auto playing = friends.GetPlayingFriends();
        REQUIRE(playing.size() == 2);
        REQUIRE(friends.InviteToGame(playing[0].steamId));
    }

    SECTION("ScreenshotUploader frame capture") {
        Shape::Community::ScreenshotUploader uploader;
        auto result = uploader.CaptureFrame("Test Screenshot");
        REQUIRE(result.success);
        REQUIRE(!result.localPath.empty());
        REQUIRE(uploader.AddSteamScreenshot("Test Screenshot"));
    }

    SECTION("ImGui Community Panels instantiation") {
        Shape::Steam::SteamContext steam;
        steam.Initialize();

        Shape::AchievementsPanel achPanel(steam.GetAchievementManager());
        REQUIRE(std::string(achPanel.GetName()) == "Achievements");
        REQUIRE(achPanel.GetEntries().size() == 20);

        Shape::WorkshopBrowserPanel wsPanel(steam.GetWorkshopManager());
        REQUIRE(std::string(wsPanel.GetName()) == "Workshop Browser");

        Shape::CommunityHubPanel hubPanel(&steam);
        REQUIRE(std::string(hubPanel.GetName()) == "Community Hub");
        REQUIRE(hubPanel.GetLeaderboardClient() != nullptr);
        REQUIRE(hubPanel.GetFriendsListManager() != nullptr);
    }
}
