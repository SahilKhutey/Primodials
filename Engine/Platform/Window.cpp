#include "Platform/Window.hpp"
#include "Input/InputSystem.hpp"
#include "Core/Assert.hpp"
#include <SDL3/SDL.h>

namespace Shape {

Window::Window(const char* title, i32 width, i32 height)
    : m_SdlWindow(nullptr)
    , m_Width(width)
    , m_Height(height) {

    // Initialize SDL Video and Input subsystems
    bool success = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);
    SHAPE_ASSERT_MSG(success, "Failed to initialize SDL: {}", SDL_GetError());

    // Create a window supporting OpenGL and resizing
    SDL_Window* window = SDL_CreateWindow(
        title,
        width,
        height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    SHAPE_ASSERT_MSG(window != nullptr, "Failed to create SDL Window: {}", SDL_GetError());

    m_SdlWindow = static_cast<void*>(window);
    SHAPE_LOG_INFO("Window initialized successfully: {}x{}", width, height);
}

Window::~Window() {
    if (m_SdlWindow) {
        SDL_DestroyWindow(static_cast<SDL_Window*>(m_SdlWindow));
    }
    SDL_Quit();
    SHAPE_LOG_INFO("Window and SDL subsystems shut down.");
}

bool Window::ProcessEvents() {
    InputSystem::Get().Update();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Send events to the InputSystem
        InputSystem::Get().ProcessEvent(&event);

        if (event.type == SDL_EVENT_QUIT) {
            return false;
        }
        else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            m_Width = event.window.data1;
            m_Height = event.window.data2;
            SHAPE_LOG_TRACE("Window resized to: {}x{}", m_Width, m_Height);
        }
    }

    return true;
}

void Window::SwapBuffers() {
    // For OpenGL, SDL3 uses SDL_GL_SwapWindow
    SDL_GL_SwapWindow(static_cast<SDL_Window*>(m_SdlWindow));
}

} // namespace Shape
