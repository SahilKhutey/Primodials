#pragma once

#include "Themes/Theme.hpp"
#include <unordered_map>
#include <memory>
#include <vector>

namespace Shape::Themes {

class ThemeManager {
public:
    ThemeManager() = default;
    ~ThemeManager() = default;

    bool Initialize(const std::string& themesDir);
    bool LoadThemeFromFile(const std::string& jsonFilePath);

    const Theme& GetActiveTheme() const { return m_ActiveTheme; }
    bool SetActiveTheme(const std::string& themeId, f32 transitionDuration = 5.0f);

    void Update(f32 dt);

    std::vector<std::string> GetAvailableThemeIds() const;
    std::vector<std::string> GetAvailableThemeDisplayNames() const;

    bool IsInTransition() const { return m_InTransition; }
    f32 GetTransitionProgress() const { return m_TransitionProgress; }

private:
    Math::Vec4f LerpVec4(const Math::Vec4f& a, const Math::Vec4f& b, f32 t) const;

    std::unordered_map<std::string, Theme> m_Themes;
    Theme m_ActiveTheme;
    Theme m_StartTheme;
    Theme m_TargetTheme;

    f32 m_TransitionDuration = 5.0f;
    f32 m_TransitionProgress = 1.0f;
    bool m_InTransition = false;
};

} // namespace Shape::Themes
