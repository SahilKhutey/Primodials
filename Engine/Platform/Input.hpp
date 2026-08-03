#pragma once

/// @file Input.hpp
/// @brief Input singleton — keyboard, mouse, gamepad state with per-frame pressed/released tracking.

#include "Platform/Keyboard.hpp"
#include "Platform/Mouse.hpp"
#include "Platform/Gamepad.hpp"
#include "Math/Vec2.hpp"
#include "Core/Platform.hpp"
#include "Core/Assert.hpp"
#include <array>
#include <string>

namespace Shape::Platform {

/// Per-frame keyboard state.
struct KeyboardState {
    static constexpr usize KEY_COUNT = static_cast<usize>(Key::Count);

    std::array<bool, KEY_COUNT> down{};
    std::array<bool, KEY_COUNT> pressed{};   ///< True only for first frame of press
    std::array<bool, KEY_COUNT> released{};  ///< True only for frame of release
    u32 modifiers = 0;

    SHAPE_NODISCARD bool is_down(Key k)     const noexcept { return down[idx(k)]; }
    SHAPE_NODISCARD bool was_pressed(Key k) const noexcept { return pressed[idx(k)]; }
    SHAPE_NODISCARD bool was_released(Key k)const noexcept { return released[idx(k)]; }

private:
    static usize idx(Key k) noexcept { return static_cast<usize>(k) & (KEY_COUNT - 1); }
};

/// Central input singleton. Call begin_frame() at the start of each game loop iteration.
class Input {
public:
    static Input& instance() noexcept {
        static Input i;
        return i;
    }

    /// Clear per-frame pressed/released/delta state. Call before polling SDL events.
    void begin_frame() noexcept;

    /// Dispatch a raw SDL3 event. Called from Window::poll_events().
    static void process_sdl_event(void* sdl_event) noexcept;

    // ── Keyboard ──────────────────────────────────────────────────────────────

    SHAPE_NODISCARD const KeyboardState& keyboard() const noexcept { return m_keyboard; }
    SHAPE_NODISCARD bool  is_key_down(Key k)     const noexcept { return m_keyboard.is_down(k); }
    SHAPE_NODISCARD bool  was_key_pressed(Key k) const noexcept { return m_keyboard.was_pressed(k); }
    SHAPE_NODISCARD bool  was_key_released(Key k)const noexcept { return m_keyboard.was_released(k); }
    SHAPE_NODISCARD const std::string& text_input() const noexcept { return m_text; }

    // ── Mouse ─────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD const MouseState& mouse()           const noexcept { return m_mouse; }
    SHAPE_NODISCARD Math::Vec2f        mouse_position()  const noexcept { return m_mouse.position; }
    SHAPE_NODISCARD Math::Vec2f        mouse_delta()     const noexcept { return m_mouse.delta; }
    SHAPE_NODISCARD Math::Vec2f        mouse_wheel()     const noexcept { return m_mouse.wheel; }
    SHAPE_NODISCARD bool is_mouse_down(MouseButton b)   const noexcept {
        return m_mouse.buttons[static_cast<usize>(b)];
    }
    SHAPE_NODISCARD bool was_mouse_pressed(MouseButton b)  const noexcept { return m_mouse_pressed[static_cast<usize>(b)]; }
    SHAPE_NODISCARD bool was_mouse_released(MouseButton b) const noexcept { return m_mouse_released[static_cast<usize>(b)]; }
    void set_relative_mouse(bool enabled) noexcept;
    void set_cursor_visible(bool visible) noexcept;

    // ── Gamepads ──────────────────────────────────────────────────────────────

    SHAPE_NODISCARD const GamepadState& gamepad(usize idx) const noexcept {
        SHAPE_ASSERT(idx < MAX_GAMEPADS);
        return m_gamepads[idx];
    }
    SHAPE_NODISCARD int  connected_gamepad_count() const noexcept;
    SHAPE_NODISCARD bool is_gamepad_down(usize idx, GamepadButton b)    const noexcept;
    SHAPE_NODISCARD bool was_gamepad_pressed(usize idx, GamepadButton b) const noexcept;
    SHAPE_NODISCARD f32  gamepad_axis(usize idx, GamepadAxis a)          const noexcept;

    // ── Config ────────────────────────────────────────────────────────────────

    void set_axis_deadzone(f32 dz) noexcept { m_deadzone = dz; }
    SHAPE_NODISCARD f32 axis_deadzone() const noexcept { return m_deadzone; }

private:
    Input() = default;

    KeyboardState m_keyboard;
    MouseState    m_mouse;
    std::array<bool, static_cast<usize>(MouseButton::Count)> m_mouse_pressed{};
    std::array<bool, static_cast<usize>(MouseButton::Count)> m_mouse_released{};
    std::array<GamepadState, MAX_GAMEPADS>                   m_gamepads{};
    std::string m_text;
    f32         m_deadzone = 0.15f;
};

} // namespace Shape::Platform
