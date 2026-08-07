#pragma once
#include "Core/Platform.hpp"
#include <chrono>

namespace Shape {
namespace Core {

class Timer {
public:
    Timer() { Reset(); }

    void Reset() {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    f32 ElapsedMilliseconds() const {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - m_Start).count() * 0.000001f;
    }

    f32 ElapsedMicroseconds() const {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};

} // namespace Core
} // namespace Shape
