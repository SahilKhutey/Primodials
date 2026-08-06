#include "ThemeManager.h"
#include "ThemeManager.hpp"
#include "Core/Logger.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

namespace ShapeEngine::Themes {

    namespace fs = std::filesystem;

    static Shape::Math::Vec4f LerpVec4(const Shape::Math::Vec4f& a, const Shape::Math::Vec4f& b, float t) {
        return Shape::Math::Vec4f(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t,
            a.w + (b.w - a.w) * t
        );
    }

    bool ThemeManager::initialize(const std::string& themesDir) {
        m_themesDir = themesDir;
        if (!fs::exists(themesDir)) {
            fs::create_directories(themesDir);
            SHAPE_LOG_INFO("Created themes directory: {}", themesDir);
        }
        discoverThemes();
        return true;
    }

    void ThemeManager::shutdown() {
        m_themes.clear();
        m_inTransition = false;
    }

    std::vector<Theme> ThemeManager::discoverThemes() {
        std::vector<Theme> result;
        m_themes.clear();

        if (!fs::exists(m_themesDir)) return result;

        for (const auto& entry : fs::directory_iterator(m_themesDir)) {
            if (!entry.is_directory()) continue;

            fs::path themeJson = entry.path() / "theme.json";
            if (!fs::exists(themeJson)) continue;

            auto theme = loadTheme(entry.path().filename().string());
            if (theme) {
                m_themes[theme->id] = theme;
                result.push_back(*theme);
                m_fileModTimes[theme->id] = fs::last_write_time(themeJson);
            }
        }

        SHAPE_LOG_INFO("Discovered {} themes from disk", m_themes.size());
        return result;
    }

    std::shared_ptr<Theme> ThemeManager::loadTheme(const std::string& id) {
        fs::path path = fs::path(m_themesDir) / id / "theme.json";
        if (!fs::exists(path)) {
            SHAPE_LOG_ERROR("Theme file not found: {}", path.string());
            return nullptr;
        }

        std::ifstream f(path);
        if (!f.is_open()) return nullptr;

        std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        auto theme = std::make_shared<Theme>();
        theme->id = id;
        theme->displayName = id;

        if (id == "deep-sea") {
            theme->displayName = "Deep Sea";
            theme->description = "Glowing creatures drift through abyssal darkness";
            theme->backgroundTopColor = Shape::Math::Vec4f(0.008f, 0.015f, 0.045f, 1.0f);
            theme->backgroundBottomColor = Shape::Math::Vec4f(0.002f, 0.005f, 0.020f, 1.0f);
            theme->speciesPalette = {
                Shape::Math::Vec4f(0.3f, 0.9f, 1.0f, 1.0f),
                Shape::Math::Vec4f(0.5f, 0.3f, 1.0f, 1.0f),
                Shape::Math::Vec4f(1.0f, 0.4f, 0.7f, 1.0f)
            };
            theme->glowIntensity = 0.85f;
            theme->particleDensity = 0.4f;
            theme->motionSpeed = 0.6f;
        } else if (id == "coral-reef") {
            theme->displayName = "Coral Reef";
            theme->description = "Tropical colors pulse through shallow waters";
            theme->backgroundTopColor = Shape::Math::Vec4f(0.02f, 0.25f, 0.45f, 1.0f);
            theme->backgroundBottomColor = Shape::Math::Vec4f(0.01f, 0.10f, 0.20f, 1.0f);
            theme->speciesPalette = {
                Shape::Math::Vec4f(1.0f, 0.5f, 0.4f, 1.0f),
                Shape::Math::Vec4f(1.0f, 0.8f, 0.3f, 1.0f),
                Shape::Math::Vec4f(0.4f, 0.9f, 0.7f, 1.0f)
            };
            theme->glowIntensity = 0.3f;
            theme->particleDensity = 0.6f;
            theme->motionSpeed = 0.9f;
        } else if (id == "aurora") {
            theme->displayName = "Aurora";
            theme->description = "Translucent lights drift across a polar sky";
            theme->backgroundTopColor = Shape::Math::Vec4f(0.05f, 0.02f, 0.12f, 1.0f);
            theme->backgroundBottomColor = Shape::Math::Vec4f(0.02f, 0.05f, 0.10f, 1.0f);
            theme->speciesPalette = {
                Shape::Math::Vec4f(0.4f, 1.0f, 0.6f, 0.8f),
                Shape::Math::Vec4f(0.5f, 0.4f, 1.0f, 0.8f),
                Shape::Math::Vec4f(0.9f, 0.4f, 1.0f, 0.8f)
            };
            theme->glowIntensity = 0.6f;
            theme->particleDensity = 0.7f;
            theme->motionSpeed = 0.5f;
        }

        SHAPE_LOG_TRACE("Loaded theme: {} ({})", theme->displayName, theme->id);
        return theme;
    }

    void ThemeManager::applyTheme(const Theme& theme) {
        m_sourceTheme = m_currentTheme;
        m_currentTheme = theme;
        m_inTransition = false;

        if (m_themeChangeCb) m_themeChangeCb(theme);
        SHAPE_LOG_INFO("Applied theme: {}", theme.displayName);
    }

    void ThemeManager::startTransition(const Theme& fromTheme, const Theme& toTheme, 
                                       float durationSeconds) {
        m_sourceTheme = fromTheme;
        m_currentTheme = fromTheme;
        m_targetTheme = toTheme;
        m_transitionDuration = (durationSeconds > 0.01f) ? durationSeconds : 0.01f;
        m_transitionElapsed = 0.0f;
        m_inTransition = true;

        SHAPE_LOG_INFO("Starting theme transition: {} -> {} ({:.1f}s)",
                 fromTheme.displayName, toTheme.displayName, durationSeconds);
    }

    void ThemeManager::update(float deltaTime) {
        if (!m_inTransition) return;

        m_transitionElapsed += deltaTime;
        float t = std::min(1.0f, m_transitionElapsed / m_transitionDuration);

        if (t >= 1.0f) {
            applyTheme(m_targetTheme);
        }
    }

    Theme ThemeManager::getCurrentInterpolatedTheme() const {
        if (!m_inTransition) return m_currentTheme;

        float t = std::min(1.0f, m_transitionElapsed / m_transitionDuration);
        t = t * t * (3.0f - 2.0f * t);

        Theme interp = m_sourceTheme;
        interp.backgroundTopColor = LerpVec4(m_sourceTheme.backgroundTopColor, 
                                              m_targetTheme.backgroundTopColor, t);
        interp.backgroundBottomColor = LerpVec4(m_sourceTheme.backgroundBottomColor, 
                                                  m_targetTheme.backgroundBottomColor, t);
        interp.glowIntensity = m_sourceTheme.glowIntensity + (m_targetTheme.glowIntensity - m_sourceTheme.glowIntensity) * t;
        interp.particleDensity = m_sourceTheme.particleDensity + (m_targetTheme.particleDensity - m_sourceTheme.particleDensity) * t;
        interp.motionSpeed = m_sourceTheme.motionSpeed + (m_targetTheme.motionSpeed - m_sourceTheme.motionSpeed) * t;
        return interp;
    }

    void ThemeManager::checkForChanges() {
        for (const auto& [id, modTime] : m_fileModTimes) {
            fs::path path = fs::path(m_themesDir) / id / "theme.json";
            if (!fs::exists(path)) continue;

            auto currentTime = fs::last_write_time(path);
            if (currentTime != modTime) {
                SHAPE_LOG_INFO("Theme {} changed on disk, reloading", id);
                auto theme = loadTheme(id);
                if (theme) {
                    m_themes[id] = theme;
                    m_fileModTimes[id] = currentTime;
                }
            }
        }
    }

    bool ThemeManager::saveTheme(const Theme& theme, const std::string& path) {
        std::ofstream f(path);
        if (!f.is_open()) return false;

        f << "{\n";
        f << "  \"id\": \"" << theme.id << "\",\n";
        f << "  \"displayName\": \"" << theme.displayName << "\",\n";
        f << "  \"description\": \"" << theme.description << "\"\n";
        f << "}\n";

        return true;
    }

} // namespace ShapeEngine::Themes

namespace Shape::Themes {

    Math::Vec4f ThemeManager::LerpVec4(const Math::Vec4f& a, const Math::Vec4f& b, f32 t) const {
        return Math::Vec4f(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t,
            a.w + (b.w - a.w) * t
        );
    }

    bool ThemeManager::Initialize(const std::string& themesDir) {
        (void)themesDir;
        Theme t1;
        t1.id = "deep-sea-bioluminescence";
        t1.displayName = "Deep Sea Bioluminescence";
        m_Themes[t1.id] = t1;

        Theme t2;
        t2.id = "coral-reef";
        t2.displayName = "Coral Reef";
        m_Themes[t2.id] = t2;

        Theme t3;
        t3.id = "aurora";
        t3.displayName = "Aurora";
        m_Themes[t3.id] = t3;

        m_ActiveTheme = t1;
        m_TransitionProgress = 1.0f;
        m_InTransition = false;
        return true;
    }

    bool ThemeManager::LoadThemeFromFile(const std::string& jsonFilePath) {
        (void)jsonFilePath;
        return true;
    }

    bool ThemeManager::SetActiveTheme(const std::string& themeId, f32 transitionDuration) {
        auto it = m_Themes.find(themeId);
        if (it == m_Themes.end()) return false;

        m_StartTheme = m_ActiveTheme;
        m_TargetTheme = it->second;
        m_TransitionDuration = (transitionDuration > 0.01f) ? transitionDuration : 0.01f;
        m_TransitionProgress = 0.0f;
        m_InTransition = true;
        return true;
    }

    void ThemeManager::Update(f32 dt) {
        if (!m_InTransition) return;

        m_TransitionProgress += dt / m_TransitionDuration;
        if (m_TransitionProgress >= 1.0f) {
            m_TransitionProgress = 1.0f;
            m_InTransition = false;
            m_ActiveTheme = m_TargetTheme;
        }
    }

    std::vector<std::string> ThemeManager::GetAvailableThemeIds() const {
        std::vector<std::string> ids;
        for (const auto& [id, theme] : m_Themes) {
            ids.push_back(id);
        }
        return ids;
    }

    std::vector<std::string> ThemeManager::GetAvailableThemeDisplayNames() const {
        std::vector<std::string> names;
        for (const auto& [id, theme] : m_Themes) {
            names.push_back(theme.displayName);
        }
        return names;
    }

} // namespace Shape::Themes
