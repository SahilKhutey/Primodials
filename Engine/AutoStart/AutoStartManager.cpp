#include "AutoStartManager.h"
#include "Core/Logger.hpp"

#include <filesystem>
#include <fstream>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#endif

namespace ShapeEngine {

#ifdef _WIN32
    static const char* REG_PATH = 
        "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    std::string AutoStartManager::getRegistrationPath() {
        return "HKCU\\" + std::string(REG_PATH);
    }

    bool AutoStartManager::enable(const std::string& executablePath, 
                                   const std::string& appName) {
        HKEY hKey;
        if (RegOpenKeyExA(HKEY_CURRENT_USER, REG_PATH, 0, 
                          KEY_SET_VALUE, &hKey) != ERROR_SUCCESS) {
            return false;
        }

        std::string command = "\"" + executablePath + "\" --minimized";
        LONG result = RegSetValueExA(hKey, appName.c_str(), 0, REG_SZ, 
                                       (const BYTE*)command.c_str(),
                                       (DWORD)(command.size() + 1));
        RegCloseKey(hKey);

        SHAPE_LOG_INFO("Auto-start enabled (Windows): {}", command);
        return result == ERROR_SUCCESS;
    }

    bool AutoStartManager::disable(const std::string& appName) {
        HKEY hKey;
        if (RegOpenKeyExA(HKEY_CURRENT_USER, REG_PATH, 0, 
                          KEY_SET_VALUE, &hKey) != ERROR_SUCCESS) {
            return false;
        }
        LONG result = RegDeleteValueA(hKey, appName.c_str());
        RegCloseKey(hKey);
        return result == ERROR_SUCCESS;
    }

    bool AutoStartManager::isEnabled(const std::string& appName) {
        HKEY hKey;
        if (RegOpenKeyExA(HKEY_CURRENT_USER, REG_PATH, 0, 
                          KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS) {
            return false;
        }
        char buffer[MAX_PATH];
        DWORD size = sizeof(buffer);
        LONG result = RegQueryValueExA(hKey, appName.c_str(), nullptr, nullptr,
                                        (LPBYTE)buffer, &size);
        RegCloseKey(hKey);
        return result == ERROR_SUCCESS;
    }

#else
    std::string AutoStartManager::getRegistrationPath() {
        return "/tmp/polygonal-primordials.desktop";
    }

    bool AutoStartManager::enable(const std::string& executablePath, const std::string& appName) {
        (void)executablePath;
        (void)appName;
        return true;
    }

    bool AutoStartManager::disable(const std::string& appName) {
        (void)appName;
        return true;
    }

    bool AutoStartManager::isEnabled(const std::string& appName) {
        (void)appName;
        return false;
    }
#endif

} // namespace ShapeEngine
