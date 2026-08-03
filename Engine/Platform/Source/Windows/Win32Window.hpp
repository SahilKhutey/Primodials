#pragma once

#include "ShapeEngine/Platform/Window.hpp"

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace Shape {

class Win32Window final : public Window {
public:
    Win32Window() = default;
    ~Win32Window() override;

    bool Create(const WindowDesc& desc) override;
    void Destroy() override;
    void PollEvents() override;
    bool ShouldClose() const override { return m_shouldClose; }

private:
    HWND m_hwnd = nullptr;
    bool m_shouldClose = false;
};

} // namespace Shape
