#pragma once

#include "ThemeValidator.h"
#include <string>
#include <functional>
#include <cstdint>

namespace ShapeEngine::Steam {

    enum class PublishStatus {
        NotStarted,
        Validating,
        Uploading,
        SettingPreview,
        Finalizing,
        Completed,
        Failed,
        Cancelled
    };

    struct PublishProgress {
        PublishStatus status = PublishStatus::NotStarted;
        float progressPercent = 0.0f;
        std::string currentStep;
        uint64_t publishedFileId = 0;
        std::string errorMessage;
    };

    class ThemePublisher {
    public:
        ThemePublisher();
        ~ThemePublisher();

        bool startPublish(const ThemeSubmission& submission);
        void cancelPublish();
        void update();

        PublishProgress getProgress() const { return m_progress; }

        using ProgressCallback = std::function<void(const PublishProgress&)>;
        using CompletedCallback = std::function<void(bool success, uint64_t fileId, 
                                                       const std::string& error)>;
        void setOnProgress(ProgressCallback cb) { m_onProgress = std::move(cb); }
        void setOnCompleted(CompletedCallback cb) { m_onCompleted = std::move(cb); }

        bool saveDraft(const ThemeSubmission& sub, const std::string& draftPath);
        bool loadDraft(ThemeSubmission& sub, const std::string& draftPath);

    private:
        void runPublishPipeline();
        bool runValidation();
        bool uploadContent();
        bool setPreviewImage();
        bool submitMetadata();
        void completePublish(bool success, const std::string& error = "");

        ThemeValidator m_validator;
        ThemeSubmission m_submission;
        PublishProgress m_progress;

        ProgressCallback m_onProgress;
        CompletedCallback m_onCompleted;

        bool m_running = false;
        bool m_cancelRequested = false;

        uint64_t m_ugcFileId = 0;
    };

} // namespace ShapeEngine::Steam
