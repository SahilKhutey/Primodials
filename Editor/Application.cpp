#include "Application.hpp"
#include "EntityInspector.hpp"
#include "SystemProfiler.hpp"
#include "Renderer2D.hpp"
#include "ECS/TransformComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include "Physics/ColliderComponent.hpp"
#include "Core/Timer.hpp"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <iostream>

namespace ShapeEditor {

Application::Application() {}

Application::~Application() {
    Shutdown();
}

bool Application::Initialize(const char* title, int width, int height) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    Uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    window = SDL_CreateWindow(title, width, height, window_flags);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; 

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    inspector = new EntityInspector();
    profiler = new SystemProfiler();
    sceneRenderer = new Renderer2D();

    // Create some dummy entities to inspect
    for (int i = 0; i < 50; ++i) {
        Shape::Entity e = world.CreateEntity();
        world.AddComponent(e, Shape::TransformComponent{
            .position = Shape::Math::Vector2f(static_cast<f32>(i % 10) * 40.0f, static_cast<f32>(i / 10) * 40.0f),
            .rotation = 0.0f,
            .scale = Shape::Math::Vector2f(1.0f, 1.0f)
        });
        
        Shape::RigidBodyComponent rb;
        rb.velocity = Shape::Math::Vector2f((i % 2 == 0 ? 100.0f : -100.0f), 200.0f);
        rb.SetMass(1.0f);
        world.AddComponent(e, rb);
        
        Shape::ColliderComponent col;
        col.type = Shape::ColliderType::Circle;
        col.radius = 10.0f;
        col.restitution = 0.8f;
        world.AddComponent(e, col);
    }

    return true;
}

void Application::ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window)) {
            running = false;
        }
    }
}

void Application::RenderUI() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    inspector->Render(world);
    profiler->Render();

    ImGui::Render();
    
    // Draw background
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
    
    // Draw 2D Scene
    sceneRenderer->Render(world, renderer);
    
    // Draw ImGui
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    
    SDL_RenderPresent(renderer);
}

void Application::Run() {
    running = true;
    Shape::Core::Timer frameTimer;
    Shape::Core::Timer stepTimer;

    // Fixed timestep
    f32 dt = 1.0f / 60.0f;
    f32 accumulator = 0.0f;

    while (running) {
        f32 frameTime = frameTimer.ElapsedMilliseconds() / 1000.0f;
        frameTimer.Reset();
        if (frameTime > 0.25f) frameTime = 0.25f; // Max frame time to avoid spiral of death
        accumulator += frameTime;

        stepTimer.Reset();
        ProcessEvents();
        profiler->PushRecord("Input Polling", stepTimer.ElapsedMilliseconds());

        stepTimer.Reset();
        while (accumulator >= dt) {
            world.Tick(dt);
            accumulator -= dt;
        }
        profiler->PushRecord("Simulation Tick", stepTimer.ElapsedMilliseconds());

        stepTimer.Reset();
        RenderUI();
        profiler->PushRecord("Render UI", stepTimer.ElapsedMilliseconds());
        
        profiler->EndFrame();
    }
}

void Application::Shutdown() {
    if (sceneRenderer) {
        delete sceneRenderer;
        sceneRenderer = nullptr;
    }
    if (profiler) {
        delete profiler;
        profiler = nullptr;
    }
    if (inspector) {
        delete inspector;
        inspector = nullptr;
    }
    if (renderer) {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

} // namespace ShapeEditor
