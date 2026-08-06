#include "WorkshopCreatorPanel.h"
#include "Authoring/ThemePreviewRenderer.h"
#include "Core/Logger.hpp"

#include <filesystem>
#include <fstream>
#include <ctime>

namespace ShapeEngine {

    namespace fs = std::filesystem;

    WorkshopCreatorPanel::WorkshopCreatorPanel(Steam::ThemePublisher* publisher, 
                                                  Themes::ThemeManager* themeManager)
        : m_publisher(publisher), m_themeManager(themeManager) {}

    void WorkshopCreatorPanel::render() {
        if (!m_visible) return;
    }

    void WorkshopCreatorPanel::startNewTheme(const std::string& baseThemeId) {
        m_workingThemeId = "user_" + std::to_string(std::time(nullptr));
        m_submission.themeDirectory = "Content/Themes/" + m_workingThemeId;
        m_submission.themeId = m_workingThemeId;
        m_submission.title = "My New Theme";
        m_submission.description = "";
        m_submission.tags = { "user-created" };

        fs::create_directories(m_submission.themeDirectory);

        if (!baseThemeId.empty() && m_themeManager) {
            auto base = m_themeManager->loadTheme(baseThemeId);
            if (base) {
                m_workingTheme = *base;
                m_workingTheme.id = m_workingThemeId;
                m_workingTheme.displayName = m_submission.title;
            }
        }
        saveThemeToDisk();
    }

    void WorkshopCreatorPanel::saveThemeToDisk() {
        if (m_themeManager) {
            m_themeManager->saveTheme(m_workingTheme, m_submission.themeDirectory + "/theme.json");
        }
        m_validationRan = false;
    }

    void WorkshopCreatorPanel::autoCapturePreviews() {
        ThemePreviewRenderer::savePreviewJpg(m_workingTheme, m_submission.themeDirectory + "/preview.jpg", 256, 256);
        ThemePreviewRenderer::savePreviewJpg(m_workingTheme, m_submission.themeDirectory + "/title.png", 512, 512);
        SHAPE_LOG_INFO("Auto-captured preview images for theme: {}", m_workingThemeId);
    }

} // namespace ShapeEngine
