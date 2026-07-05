#pragma once
#include <SDL3/SDL.h>
#include "ECS/World.hpp"

namespace ShapeEditor {

class EntityInspector;
class SystemProfiler;
class Renderer2D;

class Application {
public:
    Application();
    ~Application();

    bool Initialize(const char* title, int width, int height);
    void Run();
    void Shutdown();

private:
    void ProcessEvents();
    void RenderUI();

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;

    Shape::World world;
    EntityInspector* inspector = nullptr;
    SystemProfiler* profiler = nullptr;
    Renderer2D* sceneRenderer = nullptr;
};

} // namespace ShapeEditor
