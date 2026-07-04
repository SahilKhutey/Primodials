#pragma once
#include "Core/Platform.hpp"
#include <string_view>
#include <cstddef>

namespace Shape::Utility {

class CRC32 {
public:
    static u32 Calculate(const void* data, size_t length, u32 initialCrc = 0xFFFFFFFF);
    static u32 Calculate(std::string_view text);
};

} // namespace Shape::Utility
