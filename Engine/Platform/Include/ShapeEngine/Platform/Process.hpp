#pragma once

#include "Core/Platform.hpp"

namespace Shape {

class Process {
public:
    static u32 ID();
    static void Sleep(u32 milliseconds);
    static void Exit(i32 code);
};

} // namespace Shape
