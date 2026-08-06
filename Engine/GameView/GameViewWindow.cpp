#include "GameViewWindow.h"
#include "Core/Logger.hpp"

namespace ShapeEngine::GameView {

    GameViewWindow::GameViewWindow() = default;
    GameViewWindow::~GameViewWindow() { shutdown(); }

    bool GameViewWindow::initialize(SharedSimulationState* sharedState) {
        m_sharedState = sharedState;
        SHAPE_LOG_INFO("GameViewWindow initialized");
        return true;
    }

    void GameViewWindow::shutdown() {
        m_visible = false;
    }

    void GameViewWindow::show() {
        m_visible = true;
        SHAPE_LOG_INFO("GameViewWindow shown");
    }

    void GameViewWindow::hide() {
        m_visible = false;
        SHAPE_LOG_INFO("GameViewWindow hidden");
    }

    void GameViewWindow::toggle() {
        if (m_visible) hide(); else show();
    }

    void GameViewWindow::update(float deltaTime) {
        (void)deltaTime;
        if (!m_visible || !m_sharedState) return;

        if (m_sharedState->hasUpdatedSince(m_lastSnapshotTimestamp)) {
            m_cachedSnapshot = m_sharedState->takeSnapshot();
            m_lastSnapshotTimestamp = m_cachedSnapshot.lastUpdateTimestamp;
        }

        if (m_paused) {
            m_sharedState->requestPause(true);
        } else {
            m_sharedState->requestPause(false);
            m_sharedState->requestSpeed(m_speed);
        }
    }

    void GameViewWindow::render() {
        if (!m_visible) return;
    }

    void GameViewWindow::renderControlBar() {}
    void GameViewWindow::renderStatsOverlay() {}
    void GameViewWindow::renderEntityInspector() {}
    void GameViewWindow::renderParameterPanel() {}
    void GameViewWindow::renderDiaryPanel() {}
    void GameViewWindow::renderAchievementProgress() {}
    void GameViewWindow::handleEntityClick(int mouseX, int mouseY) {
        (void)mouseX;
        (void)mouseY;
    }

} // namespace ShapeEngine::GameView
