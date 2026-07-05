#pragma once

/// @file Mouse.hpp
/// @brief Mouse button enum and mouse state.

#include "Core/Platform.hpp"
#include "Math/Vec2.hpp"

namespace Shape::Platform {

enum class MouseButton : u8 {
    Left   = 0,
    Middle = 1,
    Right  = 2,
    X1     = 3,
    X2     = 4,
    Count  = 5
};

struct MouseState {
    Math::Vec2 position{0.0f, 0.0f};
    Math::Vec2 delta{0.0f, 0.0f};
    Math::Vec2 wheel{0.0f, 0.0f};
    bool buttons[static_cast<usize>(MouseButton::Count)] = {};
    bool inside_window   = false;
    bool relative_mode   = false;
};

} // namespace Shape::Platform
