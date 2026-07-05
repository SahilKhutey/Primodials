// Workshop.h
#pragma once
#include "Shape/Core/NonCopyable.h"
#include <cstdint>
#include <string>
#include <vector>

namespace PolygonalPrimordials {

    struct WorkshopItem {
        uint64_t id = 0;            // Steam workshop file ID
        std::string title;
        std::string description;
        std::string author;
        std::string preview_url;
        std::string download_url;
        std::vector<std::string> tags;
        uint64_t size_bytes = 0;
        int32_t  votes_up = 0;
        int32_t  votes_down = 0;
        int32_t  subscribers = 0;
        int32_t  favorites = 0;
        float    rating = 0;
        uint64_t created_time = 0;
        uint64_t updated_time = 0;
        bool     installed = false;
        bool     subscribed = false;
    };

    class Workshop : public Shape::NonCopyable {
    public:
        static Workshop& instance();

        bool initialize();
        void shutdown();

        // Browse
        void refresh_subscribed();
        void refresh_popular(int page = 0);
        void search(const std::string& query);
        const std::vector<WorkshopItem>& items() const { return m_items; }

        // Subscribe / unsubscribe
        bool subscribe(uint64_t id);
        bool unsubscribe(uint64_t id);
        bool is_subscribed(uint64_t id) const;

        // Install / uninstall
        bool install(uint64_t id);
        bool uninstall(uint64_t id);

        // Publish
        WorkshopItem create_item(const std::string& title, const std::string& description);
        bool update_item(const WorkshopItem& item);
        bool delete_item(uint64_t id);

        // Downloaded content
        std::string get_installed_path(uint64_t id) const;

    private:
        Workshop() = default;
        std::vector<WorkshopItem> m_items;
        std::vector<uint64_t> m_subscribed;
    };

} // namespace
