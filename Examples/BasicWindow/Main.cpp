#include "Core/Logger.hpp"
#include "Platform/Window.hpp"
#include "Input/InputSystem.hpp"
#include <chrono>
#include <thread>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // Initialize Engine logger
    Shape::Logger::Initialize("basic_window.log");
    SHAPE_LOG_INFO("Starting Basic Window Example...");

    // Create window
    Shape::Window window("Shape Engine - Stage 1 Verification", 800, 600);

    bool running = true;
    while (running) {
        // Poll input events and window states
        if (!window.ProcessEvents()) {
            running = false;
            break;
        }

        auto& input = Shape::InputSystem::Get();

        // Check keys
        if (input.IsKeyPressed(Shape::KeyCode::Escape)) {
            SHAPE_LOG_INFO("Escape pressed. Closing example.");
            running = false;
        }

        if (input.IsKeyPressed(Shape::KeyCode::Space)) {
            SHAPE_LOG_INFO("Spacebar pressed!");
        }

        if (input.IsMouseButtonPressed(Shape::MouseButton::Left)) {
            Shape::Math::Vector2 mousePos = input.GetMousePosition();
            SHAPE_LOG_INFO("Left Click registered at: ({}, {})", mousePos.x, mousePos.y);
        }

        // Output mouse delta if active
        Shape::Math::Vector2 delta = input.GetMouseDelta();
        if (delta.LengthSquared() > 0.0f) {
            SHAPE_LOG_TRACE("Mouse delta: ({}, {})", delta.x, delta.y);
        }

        // Simulate frame rate lock (~60fps) to keep CPU cool
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    SHAPE_LOG_INFO("Exiting example cleanly.");
    Shape::Logger::Shutdown();
    return 0;
}
