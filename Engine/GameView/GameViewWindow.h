#pragma once

#include "SharedSimulationState.h"
#include <memory>

struct SDL_Window;

namespace ShapeEngine::GameView {

    class GameViewWindow {
    public:
        GameViewWindow();
        ~GameViewWindow();

        bool initialize(SharedSimulationState* sharedState);
        void shutdown();

        void show();
        void hide();
        void toggle();

        bool isVisible() const { return m_visible; }

        void update(float deltaTime);
        void render();

        SDL_Window* getSDLWindow() const { return m_window; }

    private:
        void renderControlBar();
        void renderStatsOverlay();
        void renderEntityInspector();
        void renderParameterPanel();
        void renderDiaryPanel();
        void renderAchievementProgress();
        void handleEntityClick(int mouseX, int mouseY);

        SharedSimulationState* m_sharedState = nullptr;
        [[maybe_unused]] SDL_Window* m_window = nullptr;
        [[maybe_unused]] void* m_glContext = nullptr;

        bool m_visible = false;
        bool m_paused = false;
        float m_speed = 1.0f;
        uint64_t m_lastSnapshotTimestamp = 0;

        [[maybe_unused]] uint32_t m_selectedEntity = 0;
        SharedSimulationState::Snapshot m_cachedSnapshot;
    };

} // namespace ShapeEngine::GameView
