#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>

namespace ShapeEngine::Localization {

    class LocaleManager {
    public:
        static LocaleManager& get() {
            static LocaleManager instance;
            return instance;
        }

        bool loadLocale(const std::string& code);

        const std::string& getCurrentLocale() const { return m_currentLocale; }

        std::string get(const std::string& key) const;
        std::string getf(const char* key, ...) const;

        bool has(const std::string& key) const;

        std::vector<std::string> getAvailableLocales() const;
        std::string detectSystemLocale() const;

        using LocaleChangedCallback = std::function<void(const std::string& newLocale)>;
        void setOnLocaleChanged(LocaleChangedCallback cb) { m_changeCb = std::move(cb); }

        void setLocalesDirectory(const std::string& dir) { m_localesDirectory = dir; }

    private:
        LocaleManager() = default;

        bool loadLocaleFile(const std::string& code);
        void loadFallbackLocale();

        std::string m_currentLocale = "en";
        std::unordered_map<std::string, std::string> m_currentStrings;
        std::unordered_map<std::string, std::string> m_fallbackStrings;

        LocaleChangedCallback m_changeCb;
        std::string m_localesDirectory = "Content/Locales";
    };

} // namespace ShapeEngine::Localization
