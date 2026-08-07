// Engine/Localization/Localization.cpp
#include "Localization.h"
#include "Core/Logger.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdio>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #include <winnls.h>
#elif defined(__APPLE__)
    #include <CoreFoundation/CoreFoundation.h>
#elif defined(__linux__)
    #include <clocale>
    #include <cstdlib>
#endif

namespace ShapeEngine::Localization {

    void Localization::initialize(const Config& config) {
        m_config = config;
        m_initialized = true;

        std::string locale = config.defaultLocale;
        if (config.autoDetectSystemLocale) {
            locale = detectSystemLocale();
        }
        
        loadLocaleFile(config.defaultLocale);
        
        if (locale != config.defaultLocale) {
            loadLocale(locale);
        } else {
            m_currentLocale = locale;
        }
        
        SHAPE_LOG_INFO("Localization initialized (locale: %s)", m_currentLocale.c_str());
    }

    void Localization::shutdown() {
        m_currentStrings.clear();
        m_fallbackStrings.clear();
        m_overrides.clear();
        m_initialized = false;
    }

    bool Localization::loadLocale(const std::string& localeCode) {
        if (loadLocaleFile(localeCode)) {
            m_currentLocale = localeCode;
            SHAPE_LOG_INFO("Loaded locale: %s", localeCode.c_str());
            return true;
        }
        
        SHAPE_LOG_WARN("Failed to load locale '%s', using fallback", localeCode.c_str());
        return false;
    }

    bool Localization::loadLocaleFile(const std::string& locale) {
        std::string path = m_config.localesDirectory + "/" + locale + ".json";
        std::ifstream f(path);
        if (!f.is_open()) return false;

        std::unordered_map<std::string, std::string> strings;
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
            strings[key] = val;

            pos = valEnd + 1;
        }
        
        if (locale == m_config.defaultLocale) {
            m_fallbackStrings = strings;
        } else {
            m_currentStrings = strings;
        }
        return !strings.empty();
    }

    std::string Localization::get(const std::string& key) const {
        auto overrideIt = m_overrides.find(key);
        if (overrideIt != m_overrides.end()) {
            return overrideIt->second;
        }
        
        auto it = m_currentStrings.find(key);
        if (it != m_currentStrings.end()) {
            return it->second;
        }
        
        auto fb = m_fallbackStrings.find(key);
        if (fb != m_fallbackStrings.end()) {
            return fb->second;
        }
        
        return "[[" + key + "]]";
    }

    std::string Localization::getf(const std::string& key, 
                                     std::initializer_list<ParamValue> args) const {
        std::string fmt = get(key);
        return formatString(fmt, args);
    }

    std::string Localization::formatString(const std::string& fmt, 
                                            std::initializer_list<ParamValue> args) const {
        std::string result;
        result.reserve(fmt.size() + 32);
        
        size_t argIdx = 0;
        for (size_t i = 0; i < fmt.size(); ++i) {
            if (fmt[i] == '{' && i + 1 < fmt.size() && fmt[i + 1] == '}') {
                if (argIdx < args.size()) {
                    std::visit([&result](const auto& v) {
                        using T = std::decay_t<decltype(v)>;
                        if constexpr (std::is_same_v<T, std::string>) {
                            result += v;
                        } else if constexpr (std::is_same_v<T, int>) {
                            result += std::to_string(v);
                        } else if constexpr (std::is_floating_point_v<T>) {
                            char buf[32];
                            snprintf(buf, sizeof(buf), "%.2f", (double)v);
                            result += buf;
                        }
                    }, args.begin()[argIdx]);
                    argIdx++;
                }
                i++;
            } else if (fmt[i] == '{' && i + 2 < fmt.size() && 
                       fmt[i+1] >= '0' && fmt[i+1] <= '9' && fmt[i+2] == '}') {
                int idx = fmt[i+1] - '0';
                if (idx >= 0 && (size_t)idx < args.size()) {
                    std::visit([&result](const auto& v) {
                        using T = std::decay_t<decltype(v)>;
                        if constexpr (std::is_same_v<T, std::string>) {
                            result += v;
                        } else if constexpr (std::is_same_v<T, int>) {
                            result += std::to_string(v);
                        } else if constexpr (std::is_floating_point_v<T>) {
                            char buf[32];
                            snprintf(buf, sizeof(buf), "%.2f", (double)v);
                            result += buf;
                        }
                    }, args.begin()[idx]);
                }
                i += 2;
            } else {
                result += fmt[i];
            }
        }
        return result;
    }

    bool Localization::hasKey(const std::string& key) const {
        return m_currentStrings.count(key) > 0 || 
               m_fallbackStrings.count(key) > 0 ||
               m_overrides.count(key) > 0;
    }

    void Localization::setOverride(const std::string& key, 
                                     const std::string& value) {
        m_overrides[key] = value;
    }

    std::vector<std::string> Localization::getAvailableLocales() const {
        std::vector<std::string> result;
        namespace fs = std::filesystem;
        if (fs::exists(m_config.localesDirectory)) {
            for (const auto& entry : fs::directory_iterator(m_config.localesDirectory)) {
                if (entry.path().extension() == ".json") {
                    result.push_back(entry.path().stem().string());
                }
            }
        }
        return result;
    }

    std::string Localization::detectSystemLocale() const {
#ifdef _WIN32
        wchar_t langName[LOCALE_NAME_MAX_LENGTH];
        if (GetUserDefaultLocaleName(langName, LOCALE_NAME_MAX_LENGTH) > 0) {
            char buf[16];
            WideCharToMultiByte(CP_UTF8, 0, langName, -1, buf, sizeof(buf), 
                                  nullptr, nullptr);
            std::string s(buf);
            size_t pos = s.find('-');
            if (pos != std::string::npos) s = s.substr(0, pos);
            return s;
        }
        return "en";
#elif defined(__APPLE__)
        CFLocaleRef locale = CFLocaleCopyCurrent();
        CFStringRef code = (CFStringRef)CFLocaleGetIdentifier(locale);
        char buf[16];
        CFStringGetCString(code, buf, sizeof(buf), kCFStringEncodingUTF8);
        CFRelease(locale);
        std::string s(buf);
        size_t pos = s.find('_');
        if (pos != std::string::npos) s = s.substr(0, pos);
        return s;
#elif defined(__linux__)
        const char* lang = getenv("LANG");
        if (!lang) return "en";
        std::string s(lang);
        size_t pos = s.find('_');
        if (pos != std::string::npos) s = s.substr(0, pos);
        size_t dot = s.find('.');
        if (dot != std::string::npos) s = s.substr(0, dot);
        return s;
#else
        return "en";
#endif
    }

} // namespace ShapeEngine::Localization
