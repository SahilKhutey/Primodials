#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

namespace ShapeEngine::Steam {

    struct WorkshopRating {
        uint64_t fileId = 0;
        uint32_t stars = 5;
        bool upvoted = false;
        bool downvoted = false;
        bool isMine = false;
    };

    class WorkshopRatings {
    public:
        bool rate(uint64_t fileId, uint32_t stars);
        bool upvote(uint64_t fileId);
        bool downvote(uint64_t fileId);

        float getAverageRating(uint64_t fileId) const;
        uint32_t getRatingCount(uint64_t fileId) const;

    private:
        std::unordered_map<uint64_t, float> m_avgCache;
        std::unordered_map<uint64_t, uint32_t> m_countCache;
    };

} // namespace ShapeEngine::Steam
