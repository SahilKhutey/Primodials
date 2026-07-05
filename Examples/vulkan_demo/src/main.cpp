#include <SDL3/SDL.h>
#include "Shape/Rendering/Vulkan/VulkanRenderer.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Shape Engine - Vulkan Demo", 800, 600, SDL_WINDOW_VULKAN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    Shape::Rendering::Vulkan::VulkanRenderer renderer;
    Shape::Rendering::RendererConfig cfg;
    cfg.debug_mode = true;

    if (!renderer.initialize(window, nullptr, cfg)) {
        std::cerr << "Failed to initialize Vulkan renderer!" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        renderer.begin_frame({0.1f, 0.2f, 0.3f, 1.0f});
        // Rendering logic...
        renderer.end_frame();
    }

    renderer.shutdown();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
