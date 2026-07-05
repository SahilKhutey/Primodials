#include "Platform/Input.hpp"
#include "Core/Logger.hpp"
#include <cmath>
#include <cstring>

#if __has_include(<SDL3/SDL.h>)
#include <SDL3/SDL.h>
#define SHAPE_HAVE_SDL3 1
#else
#define SHAPE_HAVE_SDL3 0
#endif

namespace Shape::Platform {

// ─── begin_frame ─────────────────────────────────────────────────────────────

void Input::begin_frame() noexcept {
    m_keyboard.pressed.fill(false);
    m_keyboard.released.fill(false);
    m_mouse_pressed.fill(false);
    m_mouse_released.fill(false);
    m_mouse.delta = {0, 0};
    m_mouse.wheel = {0, 0};
    for (auto& gp : m_gamepads) {
        gp.pressed.fill(false);
        gp.released.fill(false);
    }
    m_text.clear();
}

// ─── process_sdl_event ───────────────────────────────────────────────────────

void Input::process_sdl_event([[maybe_unused]] void* raw) noexcept {
#if SHAPE_HAVE_SDL3
    const SDL_Event& ev = *static_cast<SDL_Event*>(raw);
    Input& in = instance();

    switch (ev.type) {
        // ── Keyboard ──────────────────────────────────────────────────────────
        case SDL_EVENT_KEY_DOWN: {
            const usize idx = static_cast<usize>(ev.key.scancode) & (KeyboardState::KEY_COUNT - 1);
            in.m_keyboard.down[idx]    = true;
            in.m_keyboard.pressed[idx] = true;
            in.m_keyboard.modifiers    = static_cast<u32>(ev.key.mod);
            break;
        }
        case SDL_EVENT_KEY_UP: {
            const usize idx = static_cast<usize>(ev.key.scancode) & (KeyboardState::KEY_COUNT - 1);
            in.m_keyboard.down[idx]     = false;
            in.m_keyboard.released[idx] = true;
            in.m_keyboard.modifiers     = static_cast<u32>(ev.key.mod);
            break;
        }
        case SDL_EVENT_TEXT_INPUT:
            if (ev.text.text) in.m_text += ev.text.text;
            break;

        // ── Mouse ─────────────────────────────────────────────────────────────
        case SDL_EVENT_MOUSE_MOTION:
            in.m_mouse.position = {ev.motion.x, ev.motion.y};
            in.m_mouse.delta   += {ev.motion.xrel, ev.motion.yrel};
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            const usize btn = static_cast<usize>(ev.button.button - 1);
            if (btn < in.m_mouse.buttons.size()) {
                in.m_mouse.buttons[btn]   = true;
                in.m_mouse_pressed[btn]   = true;
                in.m_mouse.position = {ev.button.x, ev.button.y};
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            const usize btn = static_cast<usize>(ev.button.button - 1);
            if (btn < in.m_mouse.buttons.size()) {
                in.m_mouse.buttons[btn]   = false;
                in.m_mouse_released[btn]  = true;
                in.m_mouse.position = {ev.button.x, ev.button.y};
            }
            break;
        }
        case SDL_EVENT_MOUSE_WHEEL:
            in.m_mouse.wheel = {ev.wheel.x, ev.wheel.y};
            break;

        // ── Gamepads ──────────────────────────────────────────────────────────
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
            const int idx = static_cast<int>(ev.gbutton.which);
            if (idx >= 0 && static_cast<usize>(idx) < MAX_GAMEPADS) {
                const usize b = static_cast<usize>(ev.gbutton.button);
                if (b < in.m_gamepads[idx].buttons.size()) {
                    in.m_gamepads[idx].buttons[b] = true;
                    in.m_gamepads[idx].pressed[b] = true;
                }
            }
            break;
        }
        case SDL_EVENT_GAMEPAD_BUTTON_UP: {
            const int idx = static_cast<int>(ev.gbutton.which);
            if (idx >= 0 && static_cast<usize>(idx) < MAX_GAMEPADS) {
                const usize b = static_cast<usize>(ev.gbutton.button);
                if (b < in.m_gamepads[idx].buttons.size()) {
                    in.m_gamepads[idx].buttons[b]  = false;
                    in.m_gamepads[idx].released[b] = true;
                }
            }
            break;
        }
        case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
            const int idx = static_cast<int>(ev.gaxis.which);
            if (idx >= 0 && static_cast<usize>(idx) < MAX_GAMEPADS) {
                const usize a = static_cast<usize>(ev.gaxis.axis);
                if (a < in.m_gamepads[idx].axes.size()) {
                    f32 v = ev.gaxis.value / 32767.0f;
                    if (std::abs(v) < in.m_deadzone) v = 0.0f;
                    in.m_gamepads[idx].axes[a] = v;
                }
            }
            break;
        }
        case SDL_EVENT_GAMEPAD_ADDED: {
            const int idx = static_cast<int>(ev.gdevice.which);
            if (idx >= 0 && static_cast<usize>(idx) < MAX_GAMEPADS) {
                SDL_Gamepad* gp = SDL_OpenGamepad(ev.gdevice.which);
                in.m_gamepads[idx].connected    = (gp != nullptr);
                in.m_gamepads[idx].device_index = idx;
                in.m_gamepads[idx].has_rumble   = gp && SDL_GamepadHasRumble(gp);
                SHAPE_LOG_INFO("Input", "Gamepad {} connected", idx);
            }
            break;
        }
        case SDL_EVENT_GAMEPAD_REMOVED: {
            const int idx = static_cast<int>(ev.gdevice.which);
            if (idx >= 0 && static_cast<usize>(idx) < MAX_GAMEPADS) {
                SDL_Gamepad* gp = SDL_GetGamepadFromPlayerIndex(idx);
                if (gp) SDL_CloseGamepad(gp);
                in.m_gamepads[idx] = GamepadState{};
                SHAPE_LOG_INFO("Input", "Gamepad {} disconnected", idx);
            }
            break;
        }
        default: break;
    }
#endif
}

// ─── Mouse helpers ────────────────────────────────────────────────────────────

void Input::set_relative_mouse([[maybe_unused]] bool enabled) noexcept {
#if SHAPE_HAVE_SDL3
    SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
    m_mouse.relative_mode = enabled;
#endif
}

void Input::set_cursor_visible([[maybe_unused]] bool visible) noexcept {
#if SHAPE_HAVE_SDL3
    if (visible) SDL_ShowCursor(); else SDL_HideCursor();
#endif
}

// ─── Gamepad helpers ──────────────────────────────────────────────────────────

int Input::connected_gamepad_count() const noexcept {
    int c = 0;
    for (const auto& g : m_gamepads) if (g.connected) ++c;
    return c;
}

bool Input::is_gamepad_down(usize idx, GamepadButton b) const noexcept {
    if (idx >= MAX_GAMEPADS) return false;
    return m_gamepads[idx].buttons[static_cast<usize>(b)];
}

bool Input::was_gamepad_pressed(usize idx, GamepadButton b) const noexcept {
    if (idx >= MAX_GAMEPADS) return false;
    return m_gamepads[idx].pressed[static_cast<usize>(b)];
}

f32 Input::gamepad_axis(usize idx, GamepadAxis a) const noexcept {
    if (idx >= MAX_GAMEPADS) return 0.0f;
    return m_gamepads[idx].axes[static_cast<usize>(a)];
}

// ─── Keyboard string lookup (minimal) ────────────────────────────────────────

const char* to_string(Key key) noexcept {
#if SHAPE_HAVE_SDL3
    return SDL_GetScancodeName(static_cast<SDL_Scancode>(static_cast<u32>(key)));
#else
    return "Unknown";
#endif
}

} // namespace Shape::Platform
