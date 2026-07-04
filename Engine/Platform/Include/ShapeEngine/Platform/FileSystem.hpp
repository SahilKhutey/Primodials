#pragma once

#include <filesystem>

namespace Shape {

class FileSystem {
public:
    static bool Exists(const std::filesystem::path& path);
    static bool CreateDirectory(const std::filesystem::path& path);
    static bool Remove(const std::filesystem::path& path);
};

} // namespace Shape
