// Engine/Localization/Localization.h
#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <memory>
#include <vector>
#include <initializer_list>

namespace ShapeEngine::Localization {

    /// <summary>
    /// Loads and queries localized strings from JSON files.
    /// Falls back to English (en) if key not found in current locale.
    /// </summary>
    class Localization {
    public:
        using ParamValue = std::variant<std::string, int, float, double>;
        
        struct Config {
            std::string defaultLocale = "en";
            std::string localesDirectory = "Content/Locales";
            std::string masterKeysFile = "localization_keys.json";
            bool autoDetectSystemLocale = true;
        };

        static Localization& get() {
            static Localization instance;
            return instance;
        }

        void initialize(const Config& config);
        void shutdown();

        bool loadLocale(const std::string& localeCode);
        std::string getCurrentLocale() const { return m_currentLocale; }
        std::vector<std::string> getAvailableLocales() const;

        std::string get(const std::string& key) const;
        std::string getf(const std::string& key, 
                          std::initializer_list<ParamValue> args) const;

        bool hasKey(const std::string& key) const;
        void setOverride(const std::string& key, const std::string& value);

    private:
        Localization() = default;
        ~Localization() = default;

        bool loadLocaleFile(const std::string& locale);
        std::string formatString(const std::string& fmt, 
                                   std::initializer_list<ParamValue> args) const;
        std::string detectSystemLocale() const;

        Config m_config;
        std::string m_currentLocale = "en";
        std::unordered_map<std::string, std::string> m_currentStrings;
        std::unordered_map<std::string, std::string> m_fallbackStrings;
        std::unordered_map<std::string, std::string> m_overrides;
        bool m_initialized = false;
    };

} // namespace ShapeEngine::Localization
