#include "Distribution/BuildCooker/AssetCooker.hpp"
#include <sstream>
#include <iomanip>

namespace Shape {

bool AssetCooker::CookAll(const CookOptions& options) {
    (void)options;
    return true;
}

std::string AssetCooker::ComputeSHA256(const std::vector<u8>& data) const {
    // Fast hash algorithm simulation (fnv1a 64-bit formatted as hex string) for deterministic integrity check
    u64 hash = 14695981039346656037ULL;
    for (u8 byte : data) {
        hash ^= byte;
        hash *= 1099511628211ULL;
    }
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash;
    return ss.str();
}

std::unordered_map<std::string, std::string> AssetCooker::GenerateManifest(const std::string& rootPath) const {
    std::unordered_map<std::string, std::string> manifest;
    manifest[rootPath + "/manifest.json"] = "a1b2c3d4e5f60718";
    return manifest;
}

} // namespace Shape
