#pragma once

#include "Input/InputCodes.hpp"
#include "ShapeEngine/Math/Vector2.hpp"

namespace Shape {

class InputSystem {
public:
    InputSystem();
    ~InputSystem() = default;

    // Call once per frame
    void Update();

    // SDL Event Processor
    void ProcessEvent(const void* sdlEventPtr);

    // Keyboard Queries
    bool IsKeyPressed(KeyCode key) const;
    bool IsKeyHeld(KeyCode key) const;
    bool IsKeyReleased(KeyCode key) const;

    // Mouse Queries
    bool IsMouseButtonPressed(MouseButton button) const;
    bool IsMouseButtonHeld(MouseButton button) const;
    bool IsMouseButtonReleased(MouseButton button) const;

    Math::Vector2f GetMousePosition() const { return Math::Vector2f(m_MouseX, m_MouseY); }
    Math::Vector2f GetMouseDelta() const { return Math::Vector2f(m_MouseDeltaX, m_MouseDeltaY); }

    static InputSystem& Get() {
        static InputSystem instance;
        return instance;
    }

private:
    bool m_CurrentKeys[512];
    bool m_PreviousKeys[512];

    bool m_CurrentMouse[8];
    bool m_PreviousMouse[8];

    f32 m_MouseX;
    f32 m_MouseY;
    f32 m_MouseDeltaX;
    f32 m_MouseDeltaY;
};

} // namespace Shape
