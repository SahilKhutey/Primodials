#include "Win32Window.hpp"
#include "Core/Logger.hpp"

namespace Shape {

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Win32Window* pWindow = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (uMsg == WM_CREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pWindow = reinterpret_cast<Win32Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
    }

    if (pWindow) {
        if (uMsg == WM_CLOSE) {
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

Win32Window::~Win32Window() {
    Destroy();
}


bool Win32Window::Create(const WindowDesc& desc) {
    HINSTANCE hInst = GetModuleHandle(nullptr);

    WNDCLASSEXA wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "ShapeEngineWindowClass";

    RegisterClassExA(&wc);

    m_hwnd = CreateWindowExA(
        0,
        "ShapeEngineWindowClass",
        desc.Title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        desc.Width, desc.Height,
        nullptr, nullptr, hInst, this
    );

    if (!m_hwnd) {
        SHAPE_LOG_ERROR("Win32Window: Failed to create HWND.");
        return false;
    }

    m_shouldClose = false;
    SHAPE_LOG_INFO("Win32Window: Created window '{}' ({}x{}) successfully.", desc.Title, desc.Width, desc.Height);
    return true;
}

void Win32Window::Destroy() {
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
}

void Win32Window::PollEvents() {
    MSG msg;
    while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            m_shouldClose = true;
        }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

} // namespace Shape
