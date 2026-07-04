#pragma once

#include "Core/Platform.hpp"

namespace Shape {

struct ResourceComponent {
    f32 resourceAmount = 30.0f;     // Total energy food stores
    f32 maxResourceAmount = 30.0f;
    bool isDepleted = false;

    ResourceComponent() = default;
};

} // namespace Shape
