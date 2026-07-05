// Shape/Network/Matchmaker.h
// In-process matchmaker for finding peers
#pragma once

#include "Shape/Network/INetworkBackend.h"
#include <string>
#include <vector>
#include <mutex>
#include <chrono>

namespace Shape::Network {

    struct MatchListing {
        std::string id;
        std::string host_name;
        std::string address;
        uint16_t port = 0;
        int current_players = 0;
        int max_players = 0;
        std::chrono::system_clock::time_point created_at;
        std::string game_mode;
        std::string map_name;
    };

    class Matchmaker {
    public:
        static Matchmaker& instance();

        // Local registry (in-process)
        void register_listing(const MatchListing& listing);
        void unregister_listing(const std::string& id);
        std::vector<MatchListing> list_all() const;
        std::vector<MatchListing> find_by_mode(const std::string& mode) const;

        // Heartbeat
        void prune_stale(std::chrono::seconds max_age = std::chrono::seconds(60));
        void heartbeat(const std::string& id);

    private:
        Matchmaker() = default;
        mutable std::mutex m_mutex;
        std::vector<MatchListing> m_listings;
    };

} // namespace Shape::Network
