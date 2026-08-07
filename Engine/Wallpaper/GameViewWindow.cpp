#include "Wallpaper/GameViewWindow.hpp"
#include "Core/Logger.hpp"

namespace Shape::Wallpaper {

void GameViewWindow::RenderUI(GameViewStats& stats, const std::vector<std::string>& availableThemes) {
    if (!m_Visible) return;

    // High level layout / stub state orchestration for GameView overlay
    // Trigger callbacks when user interacts with UI elements
    (void)availableThemes;

    if (stats.isPaused && m_PauseCb) {
        // Pause handling hook
    }
}

} // namespace Shape::Wallpaper
