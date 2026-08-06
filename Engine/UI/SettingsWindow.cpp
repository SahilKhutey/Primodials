#include "SettingsWindow.h"
#include "Core/Logger.hpp"
#include <SDL3/SDL.h>
#include <algorithm>

namespace ShapeEngine::UI {

    SettingsWindow::SettingsWindow() = default;
    SettingsWindow::~SettingsWindow() { shutdown(); }

    bool SettingsWindow::initialize(SDL_Window* parentWindow, void* glContext,
                                     Themes::ThemeManager* themeManager) {
        (void)parentWindow;
        m_glContext = glContext;
        m_themeManager = themeManager;
        m_initialized = true;
        SHAPE_LOG_INFO("SettingsWindow initialized (Headless / Overlay mode)");
        return true;
    }

    void SettingsWindow::shutdown() {
        m_initialized = false;
        m_visible = false;
    }

    void SettingsWindow::show() {
        if (!m_initialized) return;
        m_visible = true;
        SHAPE_LOG_INFO("SettingsWindow shown");
    }

    void SettingsWindow::hide() {
        m_visible = false;
        SHAPE_LOG_INFO("SettingsWindow hidden");
    }

    void SettingsWindow::toggle() {
        if (m_visible) hide(); else show();
    }

    void SettingsWindow::update(float deltaTime) {
        (void)deltaTime;
    }

    void SettingsWindow::render() {
        if (!m_initialized || !m_visible) return;
    }

    void SettingsWindow::renderThemeTab() {}
    void SettingsWindow::renderPerformanceTab() {}
    void SettingsWindow::renderCameraTab() {}
    void SettingsWindow::renderAudioTab() {}
    void SettingsWindow::renderParticlesTab() {}
    void SettingsWindow::renderSystemTab() {}
    void SettingsWindow::renderProfilesTab() {}
    void SettingsWindow::renderAboutSection() {}
    void SettingsWindow::renderThemeThumbnail(const Themes::Theme& theme, int size) {
        (void)theme;
        (void)size;
    }

    void SettingsWindow::applyCurrentTheme() {
        if (!m_themeManager) return;
        auto theme = m_themeManager->loadTheme(SettingsState::get().currentThemeId);
        if (theme) {
            m_previewTheme = *theme;
            m_themeManager->applyTheme(*theme);
        }
    }

    void SettingsWindow::saveSettings() {
        SettingsState::get().save("Content/_user/settings.json");
    }

} // namespace ShapeEngine::UI
