#pragma once

#include "Core/Platform.hpp"
#include <string>

namespace Shape::Community {

struct CaptureResult {
    bool success = false;
    std::string localPath;
    std::string errorMessage;
};

class ScreenshotUploader {
public:
    ScreenshotUploader() = default;
    ~ScreenshotUploader() = default;

    CaptureResult CaptureFrame(const std::string& caption = "");
    bool AddSteamScreenshot(const std::string& caption = "");
};

} // namespace Shape::Community
