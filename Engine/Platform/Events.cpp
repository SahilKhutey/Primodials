#include "Platform/Events.hpp"

// Pull in SDL3 only in this .cpp — avoids SDL headers leaking into all of Engine
#if __has_include(<SDL3/SDL.h>)
#include <SDL3/SDL.h>
#define SHAPE_HAVE_SDL3 1
#else
#define SHAPE_HAVE_SDL3 0
#endif

namespace Shape::Platform::Events {

void process([[maybe_unused]] void* sdl_event_raw, EventQueue& queue) {
#if SHAPE_HAVE_SDL3
    const SDL_Event& ev = *static_cast<SDL_Event*>(sdl_event_raw);
    Event out;
    out.timestamp = ev.common.timestamp;

    switch (ev.type) {
        case SDL_EVENT_QUIT:
            out.type = EventType::Quit;
            out.data = QuitEvent{};
            break;

        case SDL_EVENT_WINDOW_RESIZED:
            out.type = EventType::WindowResized;
            out.data = WindowResizedEvent{ev.window.data1, ev.window.data2};
            break;

        case SDL_EVENT_WINDOW_MOVED:
            out.type = EventType::WindowMoved;
            out.data = WindowMovedEvent{ev.window.data1, ev.window.data2};
            break;

        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            out.type = EventType::WindowFocusGained;
            out.data = WindowFocusEvent{true};
            break;

        case SDL_EVENT_WINDOW_FOCUS_LOST:
            out.type = EventType::WindowFocusLost;
            out.data = WindowFocusEvent{false};
            break;

        case SDL_EVENT_KEY_DOWN:
            out.type = EventType::KeyDown;
            out.data = KeyEvent{
                static_cast<Key>(ev.key.scancode),
                static_cast<u32>(ev.key.scancode),
                static_cast<u32>(ev.key.mod),
                static_cast<bool>(ev.key.repeat)
            };
            break;

        case SDL_EVENT_KEY_UP:
            out.type = EventType::KeyUp;
            out.data = KeyEvent{
                static_cast<Key>(ev.key.scancode),
                static_cast<u32>(ev.key.scancode),
                static_cast<u32>(ev.key.mod),
                false
            };
            break;

        case SDL_EVENT_TEXT_INPUT: {
            TextInputEvent te{};
            const char* src = ev.text.text;
            for (int i = 0; i < 31 && src[i]; ++i) te.text[i] = src[i];
            out.type = EventType::TextInput;
            out.data = te;
            break;
        }

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            out.type = EventType::MouseButtonDown;
            out.data = MouseButtonEvent{
                static_cast<MouseButton>(ev.button.button - 1),
                {ev.button.x, ev.button.y},
                static_cast<u32>(ev.button.mod)
            };
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            out.type = EventType::MouseButtonUp;
            out.data = MouseButtonEvent{
                static_cast<MouseButton>(ev.button.button - 1),
                {ev.button.x, ev.button.y},
                static_cast<u32>(ev.button.mod)
            };
            break;

        case SDL_EVENT_MOUSE_MOTION:
            out.type = EventType::MouseMoved;
            out.data = MouseMovedEvent{
                {ev.motion.x, ev.motion.y},
                {ev.motion.xrel, ev.motion.yrel}
            };
            break;

        case SDL_EVENT_MOUSE_WHEEL:
            out.type = EventType::MouseWheel;
            out.data = MouseWheelEvent{
                {ev.wheel.x, ev.wheel.y},
                {ev.wheel.mouse_x, ev.wheel.mouse_y}
            };
            break;

        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            out.type = EventType::GamepadButtonDown;
            out.data = GamepadButtonEvent{
                static_cast<int>(ev.gbutton.which),
                static_cast<GamepadButton>(ev.gbutton.button)
            };
            break;

        case SDL_EVENT_GAMEPAD_BUTTON_UP:
            out.type = EventType::GamepadButtonUp;
            out.data = GamepadButtonEvent{
                static_cast<int>(ev.gbutton.which),
                static_cast<GamepadButton>(ev.gbutton.button)
            };
            break;

        case SDL_EVENT_GAMEPAD_AXIS_MOTION:
            out.type = EventType::GamepadAxis;
            out.data = GamepadAxisEvent{
                static_cast<int>(ev.gaxis.which),
                static_cast<GamepadAxis>(ev.gaxis.axis),
                ev.gaxis.value / 32767.0f
            };
            break;

        case SDL_EVENT_GAMEPAD_ADDED:
            out.type = EventType::GamepadConnected;
            out.data = GamepadConnectionEvent{static_cast<int>(ev.gdevice.which), true};
            break;

        case SDL_EVENT_GAMEPAD_REMOVED:
            out.type = EventType::GamepadDisconnected;
            out.data = GamepadConnectionEvent{static_cast<int>(ev.gdevice.which), false};
            break;

        default:
            return; // Unhandled — don't push
    }

    queue.push(std::move(out));
#endif
}

} // namespace Shape::Platform::Events
