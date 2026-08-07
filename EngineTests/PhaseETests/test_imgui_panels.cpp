#include <catch2/catch_test_macros.hpp>
#include "UI/ImGuiLayer.hpp"
#include "UI/Panels/StatsPanel.hpp"
#include "UI/Panels/PopulationGraphPanel.hpp"
#include "UI/Panels/HierarchyPanel.hpp"
#include "UI/Panels/InspectorPanel.hpp"
#include "UI/Panels/ParameterPanel.hpp"

TEST_CASE("UI Subsystem - Panels and ImGuiLayer Management", "[UI][Panels]") {
    Shape::UI::ImGuiLayer uiLayer;

    SECTION("Adding panels and drawing without crash") {
        uiLayer.AddPanel(std::make_unique<Shape::UI::StatsPanel>());
        uiLayer.AddPanel(std::make_unique<Shape::UI::PopulationGraphPanel>());
        uiLayer.AddPanel(std::make_unique<Shape::UI::HierarchyPanel>());
        uiLayer.AddPanel(std::make_unique<Shape::UI::InspectorPanel>());
        uiLayer.AddPanel(std::make_unique<Shape::UI::ParameterPanel>());

        Shape::World world;
        uiLayer.DrawAllPanels(world);
        REQUIRE(true);
    }

    SECTION("StatsPanel rolling FPS calculation") {
        Shape::UI::StatsPanel stats;
        for (int i = 0; i < 120; ++i) {
            stats.PushFrameTime(1.0f / 60.0f);
        }

        REQUIRE(stats.GetAverageFPS() > 59.0f);
        REQUIRE(stats.GetAverageFPS() < 61.0f);
        REQUIRE(stats.GetLastFrameTimeMs() > 16.0f);
        REQUIRE(stats.GetLastFrameTimeMs() < 17.0f);
    }

    SECTION("PopulationGraphPanel sample history") {
        Shape::UI::PopulationGraphPanel popGraph;
        popGraph.PushSample(1.0f, 100, 20);
        popGraph.PushSample(2.0f, 120, 25);

        const auto& history = popGraph.GetHistory();
        REQUIRE(history.size() == 60);
        REQUIRE(history[0].preyCount == 100);
        REQUIRE(history[0].predatorCount == 20);
        REQUIRE(history[0].totalCount == 120);
    }

    SECTION("SimParams global access & bounds") {
        auto& params = Shape::UI::ImGuiLayer::GetSimParams();
        params.mutationRate = 0.10f;
        params.carryingCapacity = 10000.0f;

        REQUIRE(Shape::UI::ImGuiLayer::GetSimParams().mutationRate == 0.10f);
        REQUIRE(Shape::UI::ImGuiLayer::GetSimParams().carryingCapacity == 10000.0f);
    }
}
