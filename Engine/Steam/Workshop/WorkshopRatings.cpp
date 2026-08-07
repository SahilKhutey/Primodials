#include "WorkshopRatings.h"
#include "Core/Logger.hpp"

namespace ShapeEngine::Steam {

    bool WorkshopRatings::rate(uint64_t fileId, uint32_t stars) {
        if (stars < 1 || stars > 5) return false;
        m_avgCache[fileId] = static_cast<float>(stars);
        m_countCache[fileId]++;
        SHAPE_LOG_INFO("Rated theme {} with {} stars", fileId, stars);
        return true;
    }

    bool WorkshopRatings::upvote(uint64_t fileId) {
        (void)fileId;
        return true;
    }

    bool WorkshopRatings::downvote(uint64_t fileId) {
        (void)fileId;
        return true;
    }

    float WorkshopRatings::getAverageRating(uint64_t fileId) const {
        auto it = m_avgCache.find(fileId);
        return it != m_avgCache.end() ? it->second : 0.0f;
    }

    uint32_t WorkshopRatings::getRatingCount(uint64_t fileId) const {
        auto it = m_countCache.find(fileId);
        return it != m_countCache.end() ? it->second : 0;
    }

} // namespace ShapeEngine::Steam
