#pragma once

#include "Core/Platform.hpp"
#include <string>
#include <vector>
#include <functional>

namespace Shape::Wallpaper {

struct GameViewStats {
    u32 populationCount = 0;
    u32 speciesCount = 0;
    f32 fps = 30.0f;
    f32 simTimeSeconds = 0.0f;
    std::string activeThemeName = "Deep Sea Bioluminescence";
    std::string powerModeName = "Balanced (30 FPS)";
    bool isPaused = false;
    f32 speedMultiplier = 1.0f;
};

class GameViewWindow {
public:
    using PauseCallback = std::function<void(bool paused)>;
    using SpeedCallback = std::function<void(f32 speed)>;
    using ThemeCallback = std::function<void(const std::string& themeId)>;
    using ResetCallback = std::function<void()>;

    GameViewWindow() = default;
    ~GameViewWindow() = default;

    void Show() { m_Visible = true; }
    void Hide() { m_Visible = false; }
    void Toggle() { m_Visible = !m_Visible; }
    bool IsVisible() const { return m_Visible; }

    void SetPauseCallback(PauseCallback cb) { m_PauseCb = std::move(cb); }
    void SetSpeedCallback(SpeedCallback cb) { m_SpeedCb = std::move(cb); }
    void SetThemeCallback(ThemeCallback cb) { m_ThemeCb = std::move(cb); }
    void SetResetCallback(ResetCallback cb) { m_ResetCb = std::move(cb); }

    void RenderUI(GameViewStats& stats, const std::vector<std::string>& availableThemes);

private:
    bool m_Visible = false;
    PauseCallback m_PauseCb;
    SpeedCallback m_SpeedCb;
    ThemeCallback m_ThemeCb;
    ResetCallback m_ResetCb;
};

} // namespace Shape::Wallpaper
