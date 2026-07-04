#pragma once

#include "Core/Platform.hpp"

namespace Shape {

enum class KeyCode : u32 {
    Unknown = 0,
    
    // Alphabet
    A = 4, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    // Numbers
    Num1 = 30, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Num0,

    // Controls
    Return = 40,
    Escape = 41,
    Backspace = 42,
    Tab = 43,
    Space = 44,

    // Directions
    Right = 79,
    Left = 80,
    Down = 81,
    Up = 82
};

enum class MouseButton : u8 {
    None = 0,
    Left = 1,
    Middle = 2,
    Right = 3
};

} // namespace Shape
