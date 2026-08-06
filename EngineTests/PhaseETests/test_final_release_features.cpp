#include <catch2/catch_test_macros.hpp>
#include "UI/EntityPicker.hpp"
#include "UI/Panels/SpeciesEditorPanel.hpp"
#include "Renderer/HeatmapRenderer.hpp"
#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"

TEST_CASE("Final Release Features Subsystem - EntityPicker, SpeciesEditor, and HeatmapRenderer", "[Release]") {
    SECTION("EntityPicker raycast pick query precision") {
        Shape::World world;
        Shape::Entity e1 = world.CreateEntity();
        world.AddComponent<Shape::TransformComponent>(e1, Shape::TransformComponent{ .position = { 10.0f, 10.0f } });

        Shape::Entity e2 = world.CreateEntity();
        world.AddComponent<Shape::TransformComponent>(e2, Shape::TransformComponent{ .position = { 500.0f, 500.0f } });

        Shape::Entity picked = Shape::UI::EntityPicker::PickEntityAtPosition(world, { 12.0f, 11.0f }, 25.0f);
        REQUIRE(picked == e1);

        Shape::Entity missed = Shape::UI::EntityPicker::PickEntityAtPosition(world, { 250.0f, 250.0f }, 25.0f);
        REQUIRE(missed == Shape::INVALID_ENTITY);
    }

    SECTION("SpeciesEditorPanel trait configuration") {
        Shape::UI::SpeciesEditorPanel panel;
        auto& traits = panel.GetActiveTraits();
        traits.speed = 2.5f;
        traits.aggression = 0.85f;

        REQUIRE(panel.GetActiveTraits().speed == 2.5f);
        REQUIRE(panel.GetActiveTraits().aggression == 0.85f);
    }

    SECTION("HeatmapRenderer population density grid calculations") {
        Shape::World world;
        for (int i = 0; i < 50; ++i) {
            Shape::Entity e = world.CreateEntity();
            world.AddComponent<Shape::TransformComponent>(e, Shape::TransformComponent{ .position = { 10.0f, 10.0f } });
        }
        for (int i = 0; i < 10; ++i) {
            Shape::Entity e = world.CreateEntity();
            world.AddComponent<Shape::TransformComponent>(e, Shape::TransformComponent{ .position = { 500.0f, 500.0f } });
        }

        Shape::Renderer::HeatmapRenderer heatmap(50.0f);
        heatmap.CalculateDensity(world);

        const auto& cells = heatmap.GetDensityCells();
        REQUIRE(cells.size() == 2);

        f32 maxDensity = 0.0f;
        for (const auto& cell : cells) {
            if (cell.density > maxDensity) maxDensity = cell.density;
        }
        REQUIRE(maxDensity == 1.0f);
    }
}
