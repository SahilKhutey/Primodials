#include "LocaleManager.h"
#include "Core/Logger.hpp"

#include <fstream>
#include <sstream>
#include <cstdarg>
#include <cstdio>
#include <filesystem>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winnls.h>
#endif

namespace ShapeEngine::Localization {

    namespace fs = std::filesystem;

    bool LocaleManager::loadLocale(const std::string& code) {
        if (loadLocaleFile(code)) {
            m_currentLocale = code;
            if (code == "en") {
                m_fallbackStrings = m_currentStrings;
            } else if (m_fallbackStrings.empty()) {
                loadFallbackLocale();
            }
            SHAPE_LOG_INFO("Loaded locale: {}", code);
            if (m_changeCb) m_changeCb(code);
            return true;
        }

        SHAPE_LOG_WARN("Failed to load locale '{}', falling back to English", code);
        if (code != "en") loadLocale("en");
        return false;
    }

    bool LocaleManager::loadLocaleFile(const std::string& code) {
        std::string path = m_localesDirectory + "/" + code + ".json";
        std::ifstream f(path);
        if (!f.is_open()) {
            SHAPE_LOG_ERROR("Locale file not found: {}", path);
            return false;
        }

        m_currentStrings.clear();
        std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        
        size_t pos = 0;
        while (pos < content.size()) {
            size_t keyStart = content.find('"', pos);
            if (keyStart == std::string::npos) break;
            size_t keyEnd = content.find('"', keyStart + 1);
            if (keyEnd == std::string::npos) break;

            size_t colon = content.find(':', keyEnd + 1);
            if (colon == std::string::npos) break;

            size_t valStart = content.find('"', colon + 1);
            if (valStart == std::string::npos) break;
            size_t valEnd = content.find('"', valStart + 1);
            if (valEnd == std::string::npos) break;

            std::string key = content.substr(keyStart + 1, keyEnd - keyStart - 1);
            std::string val = content.substr(valStart + 1, valEnd - valStart - 1);
            m_currentStrings[key] = val;

            pos = valEnd + 1;
        }

        return !m_currentStrings.empty();
    }

    void LocaleManager::loadFallbackLocale() {
        std::string path = m_localesDirectory + "/en.json";
        std::ifstream f(path);
        if (!f.is_open()) return;

        m_fallbackStrings.clear();
        std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

        size_t pos = 0;
        while (pos < content.size()) {
            size_t keyStart = content.find('"', pos);
            if (keyStart == std::string::npos) break;
            size_t keyEnd = content.find('"', keyStart + 1);
            if (keyEnd == std::string::npos) break;

            size_t colon = content.find(':', keyEnd + 1);
            if (colon == std::string::npos) break;

            size_t valStart = content.find('"', colon + 1);
            if (valStart == std::string::npos) break;
            size_t valEnd = content.find('"', valStart + 1);
            if (valEnd == std::string::npos) break;

            std::string key = content.substr(keyStart + 1, keyEnd - keyStart - 1);
            std::string val = content.substr(valStart + 1, valEnd - valStart - 1);
            m_fallbackStrings[key] = val;

            pos = valEnd + 1;
        }
    }

    std::string LocaleManager::get(const std::string& key) const {
        auto it = m_currentStrings.find(key);
        if (it != m_currentStrings.end()) return it->second;

        auto fb = m_fallbackStrings.find(key);
        if (fb != m_fallbackStrings.end()) return fb->second;

        SHAPE_LOG_WARN("Missing translation: {}", key);
        return "[[" + key + "]]";
    }

    std::string LocaleManager::getf(const char* key, ...) const {
        std::string fmt = get(key ? key : "");
        va_list args;
        va_start(args, key);

        va_list args_copy;
        va_copy(args_copy, args);
        int size = std::vsnprintf(nullptr, 0, fmt.c_str(), args_copy);
        va_end(args_copy);

        if (size <= 0) {
            va_end(args);
            return fmt;
        }

        std::vector<char> buf(size + 1);
        std::vsnprintf(buf.data(), buf.size(), fmt.c_str(), args);
        va_end(args);

        return std::string(buf.data(), size);
    }

    bool LocaleManager::has(const std::string& key) const {
        return m_currentStrings.count(key) > 0 || m_fallbackStrings.count(key) > 0;
    }

    std::vector<std::string> LocaleManager::getAvailableLocales() const {
        std::vector<std::string> result;
        if (!fs::exists(m_localesDirectory)) return result;

        for (const auto& entry : fs::directory_iterator(m_localesDirectory)) {
            if (entry.path().extension() == ".json") {
                result.push_back(entry.path().stem().string());
            }
        }
        return result;
    }

    std::string LocaleManager::detectSystemLocale() const {
#ifdef _WIN32
        wchar_t langName[LOCALE_NAME_MAX_LENGTH];
        if (GetUserDefaultLocaleName(langName, LOCALE_NAME_MAX_LENGTH) > 0) {
            char buf[16];
            WideCharToMultiByte(CP_UTF8, 0, langName, -1, buf, sizeof(buf), nullptr, nullptr);
            std::string s(buf);
            size_t pos = s.find('-');
            if (pos != std::string::npos) s = s.substr(0, pos);
            return s;
        }
        return "en";
#else
        return "en";
#endif
    }

} // namespace ShapeEngine::Localization
