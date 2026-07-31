#include <catch2/catch_test_macros.hpp>
#include "Rendering/CameraController.h"
#include "Rendering/GameInputController.h"

TEST_CASE("CameraController & GameInputController - Viewport & Action Mapping", "[Rendering][Camera]") {
    Shape::Rendering::CameraController camera;
    Shape::Rendering::GameInputController input_ctrl(&camera, nullptr);

    Shape::Math::Vector2f screen_size{800.0f, 600.0f};

    SECTION("WorldToScreen and ScreenToWorld Roundtrip") {
        camera.SetPosition({100.0f, -50.0f});
        camera.SetZoom(2.0f);

        Shape::Math::Vector2f world_pos{150.0f, 20.0f};
        Shape::Math::Vector2f screen_pos = camera.WorldToScreen(world_pos, screen_size);
        Shape::Math::Vector2f unprojected = camera.ScreenToWorld(screen_pos, screen_size);

        REQUIRE(std::abs(unprojected.x - world_pos.x) < 0.001f);
        REQUIRE(std::abs(unprojected.y - world_pos.y) < 0.001f);
    }

    SECTION("Pan and Zoom Bounds Clamping") {
        camera.SetZoomLimits(0.5f, 4.0f);

        camera.Zoom(10.0f, {400.0f, 300.0f}, screen_size);
        REQUIRE(camera.GetZoom() == 4.0f);

        camera.Zoom(0.01f, {400.0f, 300.0f}, screen_size);
        REQUIRE(camera.GetZoom() == 0.5f);
    }

    SECTION("GameInputController Action Triggers") {
        camera.SetPosition({0.0f, 0.0f});
        camera.SetZoom(1.0f);

        input_ctrl.ExecuteAction(Shape::Rendering::InputAction::PanRight);
        camera.Update(nullptr, 1.0f); // Update camera position lerp

        REQUIRE(camera.GetPosition().x > 0.0f);

        input_ctrl.ExecuteAction(Shape::Rendering::InputAction::ResetCamera);
        camera.Update(nullptr, 1.0f);

        REQUIRE(camera.GetPosition().x == 0.0f);
        REQUIRE(camera.GetZoom() == 1.0f);
    }
}
