#pragma once

/// @file Gamepad.hpp
/// @brief Gamepad button/axis enums and GamepadState.

#include "Core/Platform.hpp"
#include <array>

namespace Shape::Platform {

enum class GamepadButton : u8 {
    South = 0,   // A / Cross
    East,        // B / Circle
    West,        // X / Square
    North,       // Y / Triangle
    Back,
    Guide,
    Start,
    LeftStick,
    RightStick,
    LeftShoulder,
    RightShoulder,
    DPadUp,
    DPadDown,
    DPadLeft,
    DPadRight,
    Misc1,
    Count
};

enum class GamepadAxis : u8 {
    LeftX        = 0,
    LeftY        = 1,
    RightX       = 2,
    RightY       = 3,
    LeftTrigger  = 4,
    RightTrigger = 5,
    Count        = 6
};

inline constexpr usize MAX_GAMEPADS = 4;

struct GamepadState {
    std::array<bool, static_cast<usize>(GamepadButton::Count)>  buttons{};
    std::array<bool, static_cast<usize>(GamepadButton::Count)>  pressed{};
    std::array<bool, static_cast<usize>(GamepadButton::Count)>  released{};
    std::array<f32,  static_cast<usize>(GamepadAxis::Count)>    axes{};
    bool connected    = false;
    int  device_index = -1;
    bool has_rumble   = false;
};

} // namespace Shape::Platform
