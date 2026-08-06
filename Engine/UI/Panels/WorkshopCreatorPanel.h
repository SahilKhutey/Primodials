#pragma once

#include "Steam/Workshop/ThemeValidator.h"
#include "Steam/Workshop/ThemePublisher.h"
#include "Themes/ThemeManager.h"

#include <memory>
#include <string>
#include <vector>

namespace ShapeEngine {

    class WorkshopCreatorPanel {
    public:
        WorkshopCreatorPanel(Steam::ThemePublisher* publisher,
                              Themes::ThemeManager* themeManager);
        ~WorkshopCreatorPanel() = default;

        void render();
        void show() { m_visible = true; }
        void hide() { m_visible = false; }
        bool isVisible() const { return m_visible; }

    private:
        enum class Mode { Create, Edit, Publish };

        void startNewTheme(const std::string& baseThemeId);
        void saveThemeToDisk();
        void autoCapturePreviews();

        [[maybe_unused]] Steam::ThemePublisher* m_publisher;
        Themes::ThemeManager* m_themeManager;

        bool m_visible = false;
        [[maybe_unused]] Mode m_mode = Mode::Create;

        Steam::ThemeSubmission m_submission;
        Themes::Theme m_workingTheme;
        std::string m_workingThemeId;

        std::vector<Steam::ValidationIssue> m_validationIssues;
        bool m_validationRan = false;
    };

} // namespace ShapeEngine
