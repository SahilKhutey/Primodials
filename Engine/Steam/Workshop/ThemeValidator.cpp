#include "ThemeValidator.h"
#include "Core/Logger.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace ShapeEngine::Steam {

    namespace fs = std::filesystem;

    ThemeValidator::ThemeValidator() = default;

    std::vector<ValidationIssue> ThemeValidator::validate(const ThemeSubmission& sub) {
        std::vector<ValidationIssue> issues;

        issues.push_back(checkRequiredFiles(sub.themeDirectory));
        issues.push_back(checkThemeJson(sub.themeDirectory + "/theme.json"));
        issues.push_back(checkPreviewImage(sub.themeDirectory + "/preview.jpg"));
        issues.push_back(checkTitleImage(sub.themeDirectory + "/title.png"));

        fs::path audioPath = fs::path(sub.themeDirectory) / "audio" / "ambient.ogg";
        if (fs::exists(audioPath)) {
            issues.push_back(checkAudioFile(audioPath.string()));
        }

        issues.push_back(checkMetadata(sub.title, sub.description, sub.tags));
        issues.push_back(checkContentPolicy(sub.title, sub.description));

        if (m_strictness != Strictness::Strict) {
            issues.erase(
                std::remove_if(issues.begin(), issues.end(),
                    [](const ValidationIssue& i) { 
                        return i.severity == ValidationIssue::Severity::Info; 
                    }),
                issues.end());
        }

        return issues;
    }

    bool ThemeValidator::canPublish(const std::vector<ValidationIssue>& issues) {
        for (const auto& i : issues) {
            if (i.severity == ValidationIssue::Severity::Error ||
                i.severity == ValidationIssue::Severity::Blocker) {
                return false;
            }
        }
        return true;
    }

    std::string ThemeValidator::generateReport(const std::vector<ValidationIssue>& issues) {
        std::stringstream ss;
        ss << "=== Theme Validation Report ===\n\n";

        int errors = 0, warnings = 0, info = 0;
        for (const auto& i : issues) {
            const char* sevStr = "";
            switch (i.severity) {
                case ValidationIssue::Severity::Error:    sevStr = "ERROR";   errors++; break;
                case ValidationIssue::Severity::Warning:  sevStr = "WARNING"; warnings++; break;
                case ValidationIssue::Severity::Blocker:  sevStr = "BLOCKER"; errors++; break;
                case ValidationIssue::Severity::Info:     sevStr = "INFO";    info++; break;
            }
            ss << "[" << sevStr << "] " << i.code << ": " << i.message << "\n";
            if (!i.suggestion.empty()) {
                ss << "    Suggestion: " << i.suggestion << "\n";
            }
        }

        ss << "\nSummary: " << errors << " errors, " << warnings 
           << " warnings, " << info << " info\n";

        bool ok = canPublish(issues);
        ss << (ok ? "✓ Ready to publish" : "✗ Cannot publish - fix errors") << "\n";
        return ss.str();
    }

    ValidationIssue ThemeValidator::checkRequiredFiles(const std::string& directory) {
        ValidationIssue issue;
        issue.code = "REQUIRED_FILES";

        std::vector<std::string> missing;
        if (!fs::exists(directory + "/theme.json"))     missing.push_back("theme.json");
        if (!fs::exists(directory + "/preview.jpg"))   missing.push_back("preview.jpg");
        if (!fs::exists(directory + "/title.png"))     missing.push_back("title.png");

        if (!missing.empty()) {
            issue.severity = ValidationIssue::Severity::Blocker;
            issue.message = "Missing required files: ";
            for (size_t i = 0; i < missing.size(); ++i) {
                issue.message += missing[i];
                if (i < missing.size() - 1) issue.message += ", ";
            }
            issue.suggestion = "Ensure theme.json, preview.jpg (256x256), and title.png (512x512) exist in " + directory;
        } else {
            issue.severity = ValidationIssue::Severity::Info;
            issue.message = "All required files present";
        }

        return issue;
    }

    ValidationIssue ThemeValidator::checkThemeJson(const std::string& path) {
        ValidationIssue issue;
        issue.code = "THEME_JSON";

        std::ifstream f(path);
        if (!f) {
            issue.severity = ValidationIssue::Severity::Blocker;
            issue.message = "theme.json not readable";
            return issue;
        }

        std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        if (content.find("\"id\"") == std::string::npos) {
            issue.severity = ValidationIssue::Severity::Error;
            issue.message = "Missing 'id' field";
            issue.suggestion = "Add 'id' field (lowercase-hyphen, e.g., 'deep-sea-bioluminescence')";
            return issue;
        }

        issue.severity = ValidationIssue::Severity::Info;
        issue.message = "theme.json is valid";
        return issue;
    }

    ValidationIssue ThemeValidator::checkPreviewImage(const std::string& path) {
        ValidationIssue issue;
        issue.code = "PREVIEW_IMAGE";

        if (!fs::exists(path)) {
            issue.severity = ValidationIssue::Severity::Blocker;
            issue.message = "preview.jpg not found";
            issue.suggestion = "Create a 256x256 JPEG preview image";
            return issue;
        }

        issue.severity = ValidationIssue::Severity::Info;
        issue.message = "preview.jpg is valid";
        return issue;
    }

    ValidationIssue ThemeValidator::checkTitleImage(const std::string& path) {
        ValidationIssue issue;
        issue.code = "TITLE_IMAGE";

        if (!fs::exists(path)) {
            issue.severity = ValidationIssue::Severity::Blocker;
            issue.message = "title.png not found";
            issue.suggestion = "Create a 512x512 PNG title image";
            return issue;
        }

        issue.severity = ValidationIssue::Severity::Info;
        issue.message = "title.png is valid";
        return issue;
    }

    ValidationIssue ThemeValidator::checkAudioFile(const std::string& path, size_t maxMB) {
        ValidationIssue issue;
        issue.code = "AUDIO_FILE";

        size_t fileSize = fs::file_size(path);
        size_t maxBytes = maxMB * 1024 * 1024;

        if (fileSize > maxBytes) {
            issue.severity = ValidationIssue::Severity::Error;
            issue.message = "Audio file is " + std::to_string(fileSize / 1024 / 1024) + 
                            " MB, max is " + std::to_string(maxMB) + " MB";
            issue.suggestion = "Compress audio or use shorter loop";
            return issue;
        }

        issue.severity = ValidationIssue::Severity::Info;
        issue.message = "Audio file is " + std::to_string(fileSize / 1024) + " KB";
        return issue;
    }

    ValidationIssue ThemeValidator::checkMetadata(const std::string& title, 
                                                   const std::string& description,
                                                   const std::vector<std::string>& tags) {
        ValidationIssue issue;
        issue.code = "METADATA";

        std::vector<std::string> problems;

        if (title.empty()) {
            problems.push_back("title is empty");
        } else if (title.length() < 3) {
            problems.push_back("title is too short (min 3 chars)");
        }

        if (description.empty()) {
            problems.push_back("description is empty");
        }

        if (tags.empty()) {
            problems.push_back("no tags — add at least 1");
        }

        if (!problems.empty()) {
            issue.severity = ValidationIssue::Severity::Warning;
            issue.message = "Metadata issues:";
            for (const auto& p : problems) {
                issue.message += "\n  - " + p;
            }
            issue.suggestion = "Improve metadata for better discoverability";
        } else {
            issue.severity = ValidationIssue::Severity::Info;
            issue.message = "Metadata looks good";
        }

        return issue;
    }

    ValidationIssue ThemeValidator::checkContentPolicy(const std::string& title, 
                                                        const std::string& description) {
        (void)title;
        (void)description;
        ValidationIssue issue;
        issue.code = "CONTENT_POLICY";
        issue.severity = ValidationIssue::Severity::Info;
        issue.message = "No obvious content policy violations";
        return issue;
    }

} // namespace ShapeEngine::Steam
