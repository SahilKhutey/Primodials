#pragma once

#include "SettingsState.h"
#include "Themes/ThemeManager.h"
#include <memory>
#include <string>

struct SDL_Window;

namespace ShapeEngine::UI {

    class SettingsWindow {
    public:
        SettingsWindow();
        ~SettingsWindow();

        bool initialize(SDL_Window* parentWindow, void* glContext,
                         Themes::ThemeManager* themeManager);

        void shutdown();

        void show();
        void hide();
        void toggle();

        bool isVisible() const { return m_visible; }

        void update(float deltaTime);
        void render();

    private:
        void renderThemeTab();
        void renderPerformanceTab();
        void renderCameraTab();
        void renderAudioTab();
        void renderParticlesTab();
        void renderSystemTab();
        void renderProfilesTab();
        void renderAboutSection();

        void renderThemeThumbnail(const Themes::Theme& theme, int size = 120);
        void applyCurrentTheme();
        void saveSettings();

        [[maybe_unused]] SDL_Window* m_window = nullptr;
        void* m_glContext = nullptr;
        Themes::ThemeManager* m_themeManager = nullptr;

        bool m_visible = false;
        bool m_initialized = false;
        [[maybe_unused]] int m_activeTab = 0;

        Themes::Theme m_previewTheme;
        [[maybe_unused]] char m_themeSearch[64] = "";
    };

} // namespace ShapeEngine::UI
