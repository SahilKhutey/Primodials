#pragma once

#include "Themes/Theme.h"
#include <string>
#include <vector>
#include <cstdint>

namespace ShapeEngine::Steam {

    struct ValidationIssue {
        enum class Severity { Info, Warning, Error, Blocker };
        Severity severity = Severity::Info;
        std::string code;
        std::string message;
        std::string field;
        std::string suggestion;
    };

    struct ThemeSubmission {
        std::string themeDirectory;
        std::string title;
        std::string description;
        std::vector<std::string> tags;
        std::string visibility = "public";

        std::string themeId;
        bool hasPreviewImage = false;
        bool hasTitleImage = false;
        bool hasAudioFile = false;
        size_t totalFileSizeBytes = 0;
    };

    class ThemeValidator {
    public:
        ThemeValidator();

        enum class Strictness { Strict, Normal, Lenient };
        void setStrictness(Strictness s) { m_strictness = s; }

        std::vector<ValidationIssue> validate(const ThemeSubmission& submission);
        bool canPublish(const std::vector<ValidationIssue>& issues);
        std::string generateReport(const std::vector<ValidationIssue>& issues);

        ValidationIssue checkRequiredFiles(const std::string& directory);
        ValidationIssue checkThemeJson(const std::string& path);
        ValidationIssue checkPreviewImage(const std::string& path);
        ValidationIssue checkTitleImage(const std::string& path);
        ValidationIssue checkAudioFile(const std::string& path, size_t maxMB = 50);
        ValidationIssue checkMetadata(const std::string& title, 
                                       const std::string& description,
                                       const std::vector<std::string>& tags);
        ValidationIssue checkContentPolicy(const std::string& title, 
                                            const std::string& description);

    private:
        Strictness m_strictness = Strictness::Normal;
        [[maybe_unused]] size_t m_maxTotalSizeBytes = 100 * 1024 * 1024;
    };

} // namespace ShapeEngine::Steam
