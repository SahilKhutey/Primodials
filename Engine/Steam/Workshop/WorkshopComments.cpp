#include "WorkshopComments.h"
#include "Core/Logger.hpp"

namespace ShapeEngine::Steam {

    void WorkshopComments::loadComments(uint64_t fileId) {
        if (m_onLoaded) m_onLoaded(fileId, m_comments);
    }

    bool WorkshopComments::postComment(uint64_t fileId, const std::string& text) {
        WorkshopComment c;
        c.fileId = fileId;
        c.text = text;
        c.timestamp = std::chrono::system_clock::now();
        m_comments.push_back(c);
        SHAPE_LOG_INFO("Posted comment to theme {}: {}", fileId, text);
        return true;
    }

} // namespace ShapeEngine::Steam
