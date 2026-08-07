#include "Community/ScreenshotUploader.hpp"

namespace Shape::Community {

CaptureResult ScreenshotUploader::CaptureFrame(const std::string& caption) {
    (void)caption;
    CaptureResult result;
    result.success = true;
    result.localPath = "screenshots/shot_sample.png";
    return result;
}

bool ScreenshotUploader::AddSteamScreenshot(const std::string& caption) {
    (void)caption;
    return true;
}

} // namespace Shape::Community
