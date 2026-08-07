#include "Core/Logger.hpp"
#include "Core/Bootstrap.hpp"
#include "Platform/Window.hpp"
#include "Rendering/SDL3Renderer.hpp"
#include "Rendering/RenderingSystem.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "Physics/ColliderComponent.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include <chrono>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    Shape::Logger::Initialize("rendering_example.log");
    SHAPE_LOG_INFO("Starting Rendering Example with Engine Integration...");

    Shape::Window window("Shape Engine - Rendering & Physics Phase 5", 800, 600);
    Shape::SDL3Renderer renderer;
    if (!renderer.Initialize(window)) {
        return -1;
    }
    
    // Initialize Core Engine
    shape::Engine& engine = shape::Engine::Get();
    if (!engine.Initialize()) {
        return -1;
    }

    // Setup World and Entities
    Shape::World& world = engine.GetWorld();
    
    // Create a floor
    Shape::Entity floor = world.CreateEntity();
    world.AddComponent(floor, Shape::TransformComponent{{400.0f, 550.0f}, 0.0f, {1.0f, 1.0f}});
    
    Shape::GeometryComponent floorGeom;
    floorGeom.vertices = {{-300, -20}, {300, -20}, {300, 20}, {-300, 20}}; // Box
    floorGeom.sideCount = 4;
    world.AddComponent(floor, floorGeom);

    Shape::ColliderComponent floorCol;
    floorCol.type = Shape::ColliderType::Polygon;
    floorCol.restitution = 0.5f;
    world.AddComponent(floor, floorCol);

    Shape::RigidBodyComponent floorBody;
    floorBody.SetMass(0.0f); // static
    world.AddComponent(floor, floorBody);


    // Create a falling bouncing triangle
    Shape::Entity tri = world.CreateEntity();
    Shape::TransformComponent triTrans{{400.0f, 100.0f}, 0.5f, {1.0f, 1.0f}};
    triTrans.previousPosition = triTrans.position;
    world.AddComponent(tri, triTrans);

    Shape::GeometryComponent triGeom;
    triGeom.vertices = {{0, -40}, {35, 20}, {-35, 20}}; // Triangle
    triGeom.sideCount = 3;
    world.AddComponent(tri, triGeom);

    Shape::ColliderComponent triCol;
    triCol.type = Shape::ColliderType::Polygon;
    triCol.restitution = 0.8f; // Bouncy
    world.AddComponent(tri, triCol);

    Shape::RigidBodyComponent triBody;
    triBody.SetMass(1.0f);
    triBody.force = {0.0f, 980.0f}; // Gravity force pre-applied (should continuously apply in a real game)
    triBody.angularVelocity = 2.0f; // Spin
    world.AddComponent(tri, triBody);


    // Create a falling circle
    Shape::Entity circle = world.CreateEntity();
    Shape::TransformComponent circleTrans{{300.0f, 50.0f}, 0.0f, {1.0f, 1.0f}};
    circleTrans.previousPosition = circleTrans.position;
    world.AddComponent(circle, circleTrans);

    Shape::ColliderComponent circleCol;
    circleCol.type = Shape::ColliderType::Circle;
    circleCol.radius = 30.0f;
    circleCol.restitution = 0.9f;
    world.AddComponent(circle, circleCol);

    Shape::RigidBodyComponent circleBody;
    circleBody.SetMass(1.5f);
    circleBody.force = {0.0f, 980.0f * 1.5f}; // Gravity
    world.AddComponent(circle, circleBody);


    Shape::RenderingSystem renderingSystem;
    renderer.SetClearColor({0.1f, 0.1f, 0.2f, 1.0f});

    auto lastTime = std::chrono::high_resolution_clock::now();
    bool running = true;
    while (running && engine.IsRunning()) {
        if (!window.ProcessEvents()) {
            running = false;
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        u64 deltaUs = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count();
        lastTime = currentTime;

        // Apply continuous gravity (since physics system clears forces)
        world.GetComponent<Shape::RigidBodyComponent>(tri).force += Shape::Math::Vector2f(0.0f, 980.0f);
        world.GetComponent<Shape::RigidBodyComponent>(circle).force += Shape::Math::Vector2f(0.0f, 980.0f * 1.5f);

        // Tick simulation
        engine.Update(deltaUs);

        // Render
        renderer.Clear();
        renderer.BeginFrame();
        renderingSystem.Render(world, renderer, engine.GetAlpha());
        renderer.EndFrame();
    }
    
    engine.Shutdown();
    renderer.Shutdown();
    Shape::Logger::Shutdown();
    return 0;
}
