#pragma once

/// @file Events.hpp
/// @brief Platform event types, EventQueue, and SDL3 event dispatch.

#include "Platform/Keyboard.hpp"
#include "Platform/Mouse.hpp"
#include "Platform/Gamepad.hpp"
#include "Math/Vec2.hpp"
#include "Core/Platform.hpp"
#include <string>
#include <variant>
#include <vector>

namespace Shape::Platform {

// ─── Event data structs ───────────────────────────────────────────────────────

struct WindowResizedEvent { int w, h; };
struct WindowMovedEvent   { int x, y; };
struct WindowFocusEvent   { bool gained; };
struct QuitEvent          {};

struct KeyEvent {
    Key  key;
    u32  scancode;
    u32  modifiers;
    bool repeat;
};

struct TextInputEvent {
    char text[32]; // UTF-8
};

struct MouseButtonEvent {
    MouseButton button;
    Math::Vec2f  position;
    u32         modifiers;
};

struct MouseMovedEvent {
    Math::Vec2f position;
    Math::Vec2f delta;
};

struct MouseWheelEvent {
    Math::Vec2f wheel;
    Math::Vec2f position;
};

struct GamepadButtonEvent {
    int           device_index;
    GamepadButton button;
};

struct GamepadAxisEvent {
    int         device_index;
    GamepadAxis axis;
    f32         value;
};

struct GamepadConnectionEvent {
    int  device_index;
    bool connected;
};

// ─── Event variant ────────────────────────────────────────────────────────────

enum class EventType : u8 {
    None,
    Quit,
    WindowResized,
    WindowMoved,
    WindowFocusGained,
    WindowFocusLost,
    KeyDown, KeyUp,
    TextInput,
    MouseButtonDown, MouseButtonUp,
    MouseMoved,
    MouseWheel,
    GamepadButtonDown, GamepadButtonUp,
    GamepadAxis,
    GamepadConnected, GamepadDisconnected,
    Count
};

using EventData = std::variant<
    std::monostate,
    QuitEvent,
    WindowResizedEvent,
    WindowMovedEvent,
    WindowFocusEvent,
    KeyEvent,
    TextInputEvent,
    MouseButtonEvent,
    MouseMovedEvent,
    MouseWheelEvent,
    GamepadButtonEvent,
    GamepadAxisEvent,
    GamepadConnectionEvent
>;

struct Event {
    EventType type      = EventType::None;
    u64       timestamp = 0; ///< SDL3 timestamp in nanoseconds
    EventData data;
};

// ─── EventQueue ───────────────────────────────────────────────────────────────

class EventQueue {
public:
    void push(Event e)  { m_events.push_back(std::move(e)); }
    void clear()        { m_events.clear(); }

    SHAPE_NODISCARD bool       empty() const noexcept { return m_events.empty(); }
    SHAPE_NODISCARD usize      size()  const noexcept { return m_events.size(); }
    SHAPE_NODISCARD const Event& operator[](usize i) const { return m_events[i]; }

    const Event* begin() const { return m_events.data(); }
    const Event* end()   const { return m_events.data() + m_events.size(); }

private:
    std::vector<Event> m_events;
};

// ─── SDL3 dispatch (implemented in Events.cpp) ────────────────────────────────
namespace Events {
    /// Translate a raw SDL_Event into our Event type and push to queue.
    /// sdl_event must be a pointer to SDL_Event.
    void process(void* sdl_event, EventQueue& queue);
}

} // namespace Shape::Platform
