#pragma once

#include "Core/Platform.hpp"

namespace Shape {

struct CPUInfo {
    u32 LogicalCores;
    u64 RAM;
    bool AVX2;
    bool SSE42;
};

class SystemInfo {
public:
    static CPUInfo GetCPUInfo();
};

} // namespace Shape
