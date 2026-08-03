#pragma once

/// @file Keyboard.hpp
/// @brief Keyboard key enums and modifier flags.

#include "Core/Platform.hpp"

namespace Shape::Platform {

/// Logical key codes (subset of SDL3 scancodes we care about).
/// Values match SDL_Scancode for direct mapping.
enum class Key : u32 {
    Unknown = 0,

    // Letters
    A = 4,  B = 5,  C = 6,  D = 7,  E = 8,  F = 9,  G = 10,
    H = 11, I = 12, J = 13, K = 14, L = 15, M = 16, N = 17,
    O = 18, P = 19, Q = 20, R = 21, S = 22, T = 23, U = 24,
    V = 25, W = 26, X = 27, Y = 28, Z = 29,

    // Numbers
    Num1 = 30, Num2 = 31, Num3 = 32, Num4 = 33, Num5 = 34,
    Num6 = 35, Num7 = 36, Num8 = 37, Num9 = 38, Num0 = 39,

    // Control
    Return    = 40,
    Escape    = 41,
    Backspace = 42,
    Tab       = 43,
    Space     = 44,

    // Function keys
    F1 = 58,  F2 = 59,  F3 = 60,  F4 = 61,  F5 = 62,
    F6 = 63,  F7 = 64,  F8 = 65,  F9 = 66,  F10 = 67,
    F11 = 68, F12 = 69,

    // Navigation
    PrintScreen = 70, ScrollLock = 71, Pause = 72,
    Insert = 73, Home = 74, PageUp = 75,
    Delete = 76, End = 77, PageDown = 78,
    Right = 79, Left = 80, Down = 81, Up = 82,

    // Modifiers
    LeftCtrl  = 224, LeftShift  = 225, LeftAlt  = 226, LeftGUI  = 227,
    RightCtrl = 228, RightShift = 229, RightAlt = 230, RightGUI = 231,

    // Misc
    CapsLock = 57, NumLock = 83, Menu = 118,

    Count = 512  // Must stay last; matches SDL_NUM_SCANCODES
};

/// Modifier key bitmask.
enum class KeyMod : u32 {
    None  = 0,
    Shift = 1 << 0,
    Ctrl  = 1 << 1,
    Alt   = 1 << 2,
    GUI   = 1 << 3,
    Caps  = 1 << 4,
    Num   = 1 << 5,
};

inline KeyMod operator|(KeyMod a, KeyMod b) noexcept {
    return static_cast<KeyMod>(static_cast<u32>(a) | static_cast<u32>(b));
}
inline bool has_mod(KeyMod mods, KeyMod test) noexcept {
    return (static_cast<u32>(mods) & static_cast<u32>(test)) != 0;
}

const char* to_string(Key key) noexcept;

} // namespace Shape::Platform
