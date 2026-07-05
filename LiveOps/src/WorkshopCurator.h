// LiveOps/src/WorkshopCurator.h
// Curate and feature community Workshop items
#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <cstdint>

namespace PolygonalPrimordials::LiveOps {

    enum class WorkshopItemType : uint8_t {
        Species, Tech, Biome, Scenario, LuaScript, TotalConversion, Tool
    };

    enum class CurationStatus : uint8_t {
        Submitted, UnderReview, Approved, Featured, Rejected, Hidden
    };

    struct WorkshopItem {
        uint64_t id = 0;
        std::string title;
        std::string description;
        WorkshopItemType type = WorkshopItemType::Species;
        std::string author_id;
        std::string author_name;
        CurationStatus status = CurationStatus::Submitted;
        int64_t file_size = 0;
        std::vector<std::string> tags;
        std::vector<std::string> preview_urls;
        int subscribers = 0;
        int favorites = 0;
        int unique_downloads = 0;
        double average_rating = 0;
        int total_ratings = 0;
        std::vector<std::string> languages;
        std::vector<std::string> compatibility;  // game versions
        std::chrono::system_clock::time_point submitted_at;
        std::chrono::system_clock::time_point updated_at;
        std::chrono::system_clock::time_point featured_until;
        std::vector<std::string> flagged_reasons;  // if any
        std::string reviewer_notes;
        std::string rejection_reason;
        int curation_score = 0;  // 0-100
    };

    class WorkshopCurator {
    public:
        // Submit (called by Workshop sync)
        WorkshopItem submit(const WorkshopItem& item);

        // Review
        bool approve(const std::string& item_id, const std::string& reviewer);
        bool reject(const std::string& item_id, const std::string& reason);
        bool hide(const std::string& item_id, const std::string& reason);
        bool flag(const std::string& item_id, const std::string& reason);

        // Feature
        bool feature(const std::string& item_id, std::chrono::system_clock::time_point until);
        bool unfeature(const std::string& item_id);

        // Curation queue
        std::vector<WorkshopItem> review_queue() const;
        std::vector<WorkshopItem> flagged() const;
        std::vector<WorkshopItem> featured() const;
        std::vector<WorkshopItem> top_rated(int n = 10) const;
        std::vector<WorkshopItem> most_subscribed(int n = 10) const;
        std::vector<WorkshopItem> trending() const;
        std::vector<WorkshopItem> recently_updated(int n = 10) const;
        std::vector<WorkshopItem> recently_added(int n = 10) const;

        // Search
        std::vector<WorkshopItem> search(const std::string& query) const;
        std::vector<WorkshopItem> by_type(WorkshopItemType t) const;
        std::vector<WorkshopItem> by_tag(const std::string& tag) const;
        std::vector<WorkshopItem> by_author(const std::string& author_id) const;

        // Auto-curation
        void auto_curate();
        int compute_curation_score(const WorkshopItem& item) const;

        // Featured rotation
        std::vector<WorkshopItem> get_featured_rotation() const;
        void rotate_featured();

        // Modder relations
        struct ModderProfile {
            std::string user_id;
            std::string name;
            int items_published = 0;
            int total_subscribers = 0;
            int total_ratings = 0;
            double average_rating = 0;
            std::chrono::system_clock::time_point first_published;
            bool verified = false;
            std::string bio;
        };
        ModderProfile get_modder(const std::string& user_id) const;
        bool verify_modder(const std::string& user_id);
        std::vector<ModderProfile> top_modders(int n = 10) const;

    private:
        std::vector<WorkshopItem> m_items;
        std::vector<ModderProfile> m_modders;
        std::vector<std::string> m_flagged_keywords;
    };

} // namespace
