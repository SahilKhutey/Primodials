#pragma once

#include "Core/Platform.hpp"

namespace Shape {

struct MonitorInfo {
    u32 Width;
    u32 Height;
    f32 DPI;
};

class Monitor {
public:
    static MonitorInfo Primary();
};

} // namespace Shape
