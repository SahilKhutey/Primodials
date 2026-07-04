#pragma once

#include "Core/Platform.hpp"

namespace Shape {

class Window {
public:
    Window(const char* title, i32 width, i32 height);
    ~Window();

    // Polls window messages. Returns false if window requested close (Quit).
    bool ProcessEvents();

    void SwapBuffers();

    i32 GetWidth() const { return m_Width; }
    i32 GetHeight() const { return m_Height; }

    void* GetNativeWindow() const { return m_SdlWindow; }

private:
    void* m_SdlWindow; // Keeps SDL header dependency out of engine headers
    i32 m_Width;
    i32 m_Height;
};

} // namespace Shape
