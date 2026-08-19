# Steamworks SDK Integration Guide

Integration notes for the native C++23 engine when packaging for Steam Direct.

---

## 📦 SETUP & LINKING

In `CMakeLists.txt`:

```cmake
# Optional Steamworks target
option(ENABLE_STEAMWORKS "Build with Steamworks SDK" OFF)

if(ENABLE_STEAMWORKS)
    add_definitions(-DUSE_STEAMWORKS)
    include_directories(vendor/steamworks/public)
    target_link_libraries(PolygonalPrimordials PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/steamworks/redistributable_bin/steam_api64.lib)
endif()
```

---

## 🔑 LIFECYCLE WRAPPER (C++23)

```cpp
#ifdef USE_STEAMWORKS
#include <steam/steam_api.h>

class SteamManager {
public:
    static bool init() {
        if (!SteamAPI_Init()) {
            return false;
        }
        return true;
    }

    static void update() {
        SteamAPI_RunCallbacks();
    }

    static void shutdown() {
        SteamAPI_Shutdown();
    }

    static void unlockAchievement(const char* id) {
        if (SteamUserStats()) {
            SteamUserStats()->SetAchievement(id);
            SteamUserStats()->StoreStats();
        }
    }
};
#endif
```
