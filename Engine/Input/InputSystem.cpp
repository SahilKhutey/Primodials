#include "Input/InputSystem.hpp"
#include <SDL3/SDL.h>
#include <cstring>

namespace Shape {

InputSystem::InputSystem()
    : m_MouseX(0.0f)
    , m_MouseY(0.0f)
    , m_MouseDeltaX(0.0f)
    , m_MouseDeltaY(0.0f) {
    std::memset(m_CurrentKeys, 0, sizeof(m_CurrentKeys));
    std::memset(m_PreviousKeys, 0, sizeof(m_PreviousKeys));
    std::memset(m_CurrentMouse, 0, sizeof(m_CurrentMouse));
    std::memset(m_PreviousMouse, 0, sizeof(m_PreviousMouse));
}

void InputSystem::Update() {
    std::memcpy(m_PreviousKeys, m_CurrentKeys, sizeof(m_CurrentKeys));
    std::memcpy(m_PreviousMouse, m_CurrentMouse, sizeof(m_CurrentMouse));
    
    // Delta should reset each frame unless updated by events
    m_MouseDeltaX = 0.0f;
    m_MouseDeltaY = 0.0f;
}

void InputSystem::ProcessEvent(const void* sdlEventPtr) {
    const SDL_Event* event = static_cast<const SDL_Event*>(sdlEventPtr);

    switch (event->type) {
        case SDL_EVENT_KEY_DOWN: {
            u32 scancode = static_cast<u32>(event->key.scancode);
            if (scancode < 512) {
                m_CurrentKeys[scancode] = true;
            }
            break;
        }
        case SDL_EVENT_KEY_UP: {
            u32 scancode = static_cast<u32>(event->key.scancode);
            if (scancode < 512) {
                m_CurrentKeys[scancode] = false;
            }
            break;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            m_MouseX = event->motion.x;
            m_MouseY = event->motion.y;
            m_MouseDeltaX = event->motion.xrel;
            m_MouseDeltaY = event->motion.yrel;
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            u8 button = event->button.button;
            if (button < 8) {
                m_CurrentMouse[button] = true;
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            u8 button = event->button.button;
            if (button < 8) {
                m_CurrentMouse[button] = false;
            }
            break;
        }
        default:
            break;
    }
}

bool InputSystem::IsKeyPressed(KeyCode key) const {
    u32 scancode = static_cast<u32>(key);
    return scancode < 512 && m_CurrentKeys[scancode] && !m_PreviousKeys[scancode];
}

bool InputSystem::IsKeyHeld(KeyCode key) const {
    u32 scancode = static_cast<u32>(key);
    return scancode < 512 && m_CurrentKeys[scancode];
}

bool InputSystem::IsKeyReleased(KeyCode key) const {
    u32 scancode = static_cast<u32>(key);
    return scancode < 512 && !m_CurrentKeys[scancode] && m_PreviousKeys[scancode];
}

bool InputSystem::IsMouseButtonPressed(MouseButton button) const {
    u8 idx = static_cast<u8>(button);
    return idx < 8 && m_CurrentMouse[idx] && !m_PreviousMouse[idx];
}

bool InputSystem::IsMouseButtonHeld(MouseButton button) const {
    u8 idx = static_cast<u8>(button);
    return idx < 8 && m_CurrentMouse[idx];
}

bool InputSystem::IsMouseButtonReleased(MouseButton button) const {
    u8 idx = static_cast<u8>(button);
    return idx < 8 && !m_CurrentMouse[idx] && m_PreviousMouse[idx];
}

} // namespace Shape
