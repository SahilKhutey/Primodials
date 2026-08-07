#pragma once

#include "Core/Platform.hpp"

namespace Shape {

class Clock {
public:
    static f64 Seconds();
    static f64 Milliseconds();
};

} // namespace Shape
