#pragma once

#include "Core/Platform.hpp"

namespace Shape::Steam {

struct SteamAppConfig {
    static constexpr u32 APP_ID = 480; // Default Spacewar App ID for dev testing
    static constexpr u32 DEPOT_COMMON = 1001;
    static constexpr u32 DEPOT_WINDOWS = 1002;
    static constexpr u32 DEPOT_LINUX = 1003;
    static constexpr u32 DEPOT_MACOS = 1004;
    static constexpr u32 DEPOT_WORKSHOP = 1005;

    static constexpr usize ACHIEVEMENT_COUNT = 20;
    static constexpr u64 CLOUD_QUOTA_BYTES = 100 * 1024 * 1024; // 100 MB

    static constexpr const char* APP_NAME = "Primordials";
    static constexpr const char* APP_VERSION = "2.0.0";
    static constexpr const char* BUILD_ID = "build_20260803_001";
};

} // namespace Shape::Steam
