#pragma once

#include "Core/Platform.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace Shape {

struct CookOptions {
    std::string sourceRoot = "Content";
    std::string outputRoot = "build/content";
    bool generateManifest = true;
};

class AssetCooker {
public:
    AssetCooker() = default;
    ~AssetCooker() = default;

    bool CookAll(const CookOptions& options);
    std::string ComputeSHA256(const std::vector<u8>& data) const;
    std::unordered_map<std::string, std::string> GenerateManifest(const std::string& rootPath) const;
};

} // namespace Shape
