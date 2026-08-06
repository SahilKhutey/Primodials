#include <catch2/catch_test_macros.hpp>
#include "Diary/EcosystemDiary.h"
#include "GameView/SharedSimulationState.h"
#include "Steam/Workshop/WorkshopThemeManager.h"
#include "AutoStart/AutoStartManager.h"
#include <cstdio>

using namespace ShapeEngine;

TEST_CASE("Diary records extinctions correctly", "[diary]") {
    Diary::EcosystemDiary diary;
    std::string testPath = "test_diary.json";
    REQUIRE(diary.initialize(testPath));

    diary.notifySpeciesChange(0, 10);
    diary.notifySpeciesChange(0, 0);

    auto recent = diary.getRecent(10);
    REQUIRE(recent.size() >= 1);

    bool foundExtinction = false;
    for (const auto& e : recent) {
        if (e.type == Diary::EventType::FirstExtinction) {
            foundExtinction = true;
            REQUIRE(e.speciesId == 0);
            REQUIRE(e.countBefore == 10);
        }
    }
    REQUIRE(foundExtinction);
    diary.shutdown();
    std::remove(testPath.c_str());
}

TEST_CASE("SharedSimulationState snapshot and pause flow", "[shared]") {
    GameView::SharedSimulationState& shared = GameView::SharedSimulationState::get();
    shared.initialize();

    shared.requestPause(true);
    REQUIRE(shared.isPauseRequested());

    shared.requestPause(false);
    REQUIRE_FALSE(shared.isPauseRequested());

    shared.requestSpeed(2.0f);
    REQUIRE(shared.getRequestedSpeed() == 2.0f);
    shared.shutdown();
}

TEST_CASE("WorkshopThemeManager subscribe and unsubscribe", "[workshop]") {
    Steam::WorkshopThemeManager manager;
    REQUIRE(manager.initialize("Content/Themes/_test_workshop"));

    REQUIRE(manager.subscribe(12345));
    REQUIRE(manager.unsubscribe(12345));

    manager.shutdown();
}

TEST_CASE("AutoStartManager returns registration path", "[autostart]") {
    std::string path = AutoStartManager::getRegistrationPath();
    REQUIRE_FALSE(path.empty());
}
