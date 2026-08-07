#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include <chrono>

namespace ShapeEngine::Steam {

    struct WorkshopComment {
        uint64_t id = 0;
        uint64_t fileId = 0;
        uint64_t authorSteamId = 0;
        std::string authorName;
        std::string text;
        std::chrono::system_clock::time_point timestamp;
        uint32_t upvotes = 0;
        bool isOwn = false;
    };

    class WorkshopComments {
    public:
        void loadComments(uint64_t fileId);
        bool postComment(uint64_t fileId, const std::string& text);

        const std::vector<WorkshopComment>& getComments() const { return m_comments; }

        using CommentsCallback = std::function<void(uint64_t fileId, const std::vector<WorkshopComment>&)>;
        void setOnLoaded(CommentsCallback cb) { m_onLoaded = std::move(cb); }

    private:
        std::vector<WorkshopComment> m_comments;
        CommentsCallback m_onLoaded;
    };

} // namespace ShapeEngine::Steam
