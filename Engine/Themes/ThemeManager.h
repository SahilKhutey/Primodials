#pragma once

#include "Theme.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <filesystem>

namespace ShapeEngine::Themes {

    class ThemeManager {
    public:
        bool initialize(const std::string& themesDir);
        void shutdown();

        std::vector<Theme> discoverThemes();

        std::shared_ptr<Theme> loadTheme(const std::string& id);

        void applyTheme(const Theme& theme);

        void startTransition(const Theme& fromTheme, const Theme& toTheme, 
                            float durationSeconds = 5.0f);

        void update(float deltaTime);

        Theme getCurrentInterpolatedTheme() const;

        void checkForChanges();

        bool saveTheme(const Theme& theme, const std::string& path);

        const Theme& getCurrentTheme() const { return m_currentTheme; }

        using ThemeChangeCallback = std::function<void(const Theme& newTheme)>;
        void setOnThemeChange(ThemeChangeCallback cb) { m_themeChangeCb = std::move(cb); }

    private:
        std::string m_themesDir;
        std::unordered_map<std::string, std::shared_ptr<Theme>> m_themes;
        Theme m_currentTheme;
        Theme m_sourceTheme;
        Theme m_targetTheme;
        float m_transitionDuration = 0.0f;
        float m_transitionElapsed = 0.0f;
        bool m_inTransition = false;

        ThemeChangeCallback m_themeChangeCb;

        std::unordered_map<std::string, std::filesystem::file_time_type> m_fileModTimes;
    };

} // namespace ShapeEngine::Themes
