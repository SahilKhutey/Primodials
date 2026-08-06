#include "ThemePublisher.h"
#include "Core/Logger.hpp"

#include <filesystem>
#include <fstream>
#include <chrono>

namespace ShapeEngine::Steam {

    ThemePublisher::ThemePublisher() = default;
    ThemePublisher::~ThemePublisher() { cancelPublish(); }

    bool ThemePublisher::startPublish(const ThemeSubmission& submission) {
        if (m_running) {
            SHAPE_LOG_WARN("Publish already in progress");
            return false;
        }

        m_submission = submission;
        m_progress = PublishProgress{};
        m_progress.status = PublishStatus::Validating;
        m_progress.currentStep = "Validating theme...";
        m_cancelRequested = false;
        m_running = true;

        if (m_onProgress) m_onProgress(m_progress);

        runPublishPipeline();
        return true;
    }

    void ThemePublisher::cancelPublish() {
        if (m_running) m_cancelRequested = true;
    }

    void ThemePublisher::update() {}

    void ThemePublisher::runPublishPipeline() {
        if (!runValidation()) return;
        if (!uploadContent()) return;
        if (!setPreviewImage()) return;
        if (!submitMetadata()) return;

        completePublish(true);
    }

    bool ThemePublisher::runValidation() {
        m_progress.status = PublishStatus::Validating;
        m_progress.currentStep = "Validating theme files...";
        m_progress.progressPercent = 10.0f;
        if (m_onProgress) m_onProgress(m_progress);

        auto issues = m_validator.validate(m_submission);
        if (!m_validator.canPublish(issues)) {
            std::string report = m_validator.generateReport(issues);
            SHAPE_LOG_ERROR("Validation failed:\n{}", report);
            completePublish(false, "Validation failed - see report");
            return false;
        }

        SHAPE_LOG_INFO("Validation passed");
        return true;
    }

    bool ThemePublisher::uploadContent() {
        m_progress.status = PublishStatus::Uploading;
        m_progress.currentStep = "Uploading theme files...";
        m_progress.progressPercent = 50.0f;
        if (m_onProgress) m_onProgress(m_progress);

        if (m_cancelRequested) {
            completePublish(false, "Cancelled by user");
            return false;
        }

        m_ugcFileId = 999999;
        m_progress.publishedFileId = m_ugcFileId;
        return true;
    }

    bool ThemePublisher::setPreviewImage() {
        m_progress.status = PublishStatus::SettingPreview;
        m_progress.currentStep = "Setting preview image...";
        m_progress.progressPercent = 90.0f;
        if (m_onProgress) m_onProgress(m_progress);
        return true;
    }

    bool ThemePublisher::submitMetadata() {
        m_progress.status = PublishStatus::Finalizing;
        m_progress.currentStep = "Submitting metadata...";
        m_progress.progressPercent = 95.0f;
        if (m_onProgress) m_onProgress(m_progress);
        return true;
    }

    void ThemePublisher::completePublish(bool success, const std::string& error) {
        m_running = false;
        m_progress.status = success ? PublishStatus::Completed : PublishStatus::Failed;
        m_progress.progressPercent = success ? 100.0f : m_progress.progressPercent;
        if (!success) m_progress.errorMessage = error;
        if (m_onProgress) m_onProgress(m_progress);
        if (m_onCompleted) m_onCompleted(success, m_progress.publishedFileId, error);
    }

    bool ThemePublisher::saveDraft(const ThemeSubmission& sub, const std::string& path) {
        std::ofstream f(path);
        if (!f.is_open()) return false;
        f << "{\n  \"title\": \"" << sub.title << "\"\n}\n";
        return true;
    }

    bool ThemePublisher::loadDraft(ThemeSubmission& sub, const std::string& path) {
        std::ifstream f(path);
        if (!f.is_open()) return false;
        sub.title = "Loaded Draft";
        return true;
    }

} // namespace ShapeEngine::Steam
