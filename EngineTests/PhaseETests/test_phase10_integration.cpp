// EngineTests/PhaseETests/test_phase10_integration.cpp
#include <catch2/catch_test_macros.hpp>
#include "UI/Achievements/AchievementPanel.h"
#include "UI/Onboarding/TutorialOverlay.h"
#include "UI/Performance/PerformanceOverlay.h"
#include "UI/Heatmap/HeatmapToggle.h"

TEST_CASE("AchievementPanel load and unlock", "[phase10]") {
    ShapeEngine::UI::AchievementPanel::Config cfg;
    ShapeEngine::UI::AchievementPanel panel(cfg);

    std::vector<ShapeEngine::UI::AchievementPanel::Achievement> list = {
        {"first_launch", "First Steps", "Launch the simulation", "", "", false, 0.0f, 0, false},
        {"pop_100", "Century", "Reach population 100", "", "", false, 0.5f, 0, false}
    };

    panel.loadAchievements(list);
    REQUIRE(panel.getTotalCount() == 2);
    REQUIRE(panel.getUnlockedCount() == 0);
    REQUIRE(panel.getCompletionPercent() == 0.0f);

    bool callbackFired = false;
    panel.setOnUnlock([&](const ShapeEngine::UI::AchievementPanel::Achievement& ach) {
        callbackFired = true;
        REQUIRE(ach.id == "first_launch");
    });

    panel.unlockAchievement("first_launch");
    REQUIRE(panel.getUnlockedCount() == 1);
    REQUIRE(panel.getCompletionPercent() == 50.0f);
    REQUIRE(callbackFired);
}

TEST_CASE("TutorialOverlay guided navigation", "[phase10]") {
    ShapeEngine::UI::TutorialOverlay::Config cfg;
    ShapeEngine::UI::TutorialOverlay tutorial(cfg);

    REQUIRE_FALSE(tutorial.isActive());
    tutorial.start();
    REQUIRE(tutorial.isActive());
    REQUIRE(tutorial.getCurrentStepIndex() == 0);

    tutorial.nextStep();
    REQUIRE(tutorial.getCurrentStepIndex() == 1);

    tutorial.previousStep();
    REQUIRE(tutorial.getCurrentStepIndex() == 0);

    bool skipFired = false;
    tutorial.setOnSkipped([&]() { skipFired = true; });
    tutorial.skip();

    REQUIRE_FALSE(tutorial.isActive());
    REQUIRE(tutorial.isSkipped());
    REQUIRE(skipFired);
}

TEST_CASE("PerformanceOverlay frame recording and stats", "[phase10]") {
    ShapeEngine::UI::PerformanceOverlay::Config cfg;
    ShapeEngine::UI::PerformanceOverlay perf(cfg);

    REQUIRE_FALSE(perf.isVisible());
    perf.show();
    REQUIRE(perf.isVisible());

    for (int i = 0; i < 60; ++i) {
        perf.recordFrame(0.0166f, 1500, 1.0f);
    }

    REQUIRE(perf.getCurrentEntities() == 1500);
    REQUIRE(perf.getAvgFPS() > 50.0f);
    REQUIRE(perf.getAvgFrameTime() < 20.0f);
}

TEST_CASE("HeatmapToggle mode switching and legend", "[phase10]") {
    ShapeEngine::UI::HeatmapToggle::Config cfg;
    ShapeEngine::UI::HeatmapToggle heatmap(cfg);

    REQUIRE(heatmap.getMode() == ShapeEngine::UI::HeatmapMode::None);
    REQUIRE_FALSE(heatmap.isLegendVisible());

    heatmap.setMode(ShapeEngine::UI::HeatmapMode::PopulationDensity);
    heatmap.showLegend();

    REQUIRE(heatmap.getMode() == ShapeEngine::UI::HeatmapMode::PopulationDensity);
    REQUIRE(heatmap.isLegendVisible());
    REQUIRE(heatmap.getModeName(heatmap.getMode()) == "Population Density");
}
