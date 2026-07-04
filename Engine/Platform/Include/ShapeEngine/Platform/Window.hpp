#pragma once

#include "Core/Platform.hpp"
#include <string>

namespace Shape {

struct WindowDesc {
    std::string Title = "Shape Engine";
    u32 Width = 1280;
    u32 Height = 720;
    bool Resizable = true;
};

class Window {
public:
    virtual ~Window() = default;
    virtual bool Create(const WindowDesc& desc) = 0;
    virtual void Destroy() = 0;
    virtual void PollEvents() = 0;
    virtual bool ShouldClose() const = 0;
};

} // namespace Shape
