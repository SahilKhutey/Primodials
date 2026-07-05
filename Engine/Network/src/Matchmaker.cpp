// Shape/Network/Matchmaker.cpp
#include "Shape/Network/Matchmaker.h"
#include <algorithm>

namespace Shape::Network {

    Matchmaker& Matchmaker::instance() {
        static Matchmaker inst;
        return inst;
    }

    void Matchmaker::register_listing(const MatchListing& listing) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_listings.push_back(listing);
    }

    void Matchmaker::unregister_listing(const std::string& id) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_listings.erase(std::remove_if(m_listings.begin(), m_listings.end(),
            [&](const MatchListing& l) { return l.id == id; }), m_listings.end());
    }

    std::vector<MatchListing> Matchmaker::list_all() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_listings;
    }

    std::vector<MatchListing> Matchmaker::find_by_mode(const std::string& mode) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<MatchListing> result;
        for (const auto& l : m_listings) {
            if (l.game_mode == mode) result.push_back(l);
        }
        return result;
    }

    void Matchmaker::prune_stale(std::chrono::seconds max_age) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto now = std::chrono::system_clock::now();
        m_listings.erase(std::remove_if(m_listings.begin(), m_listings.end(),
            [&](const MatchListing& l) { return (now - l.created_at) > max_age; }), m_listings.end());
    }

    void Matchmaker::heartbeat(const std::string& id) {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& l : m_listings) {
            if (l.id == id) {
                l.created_at = std::chrono::system_clock::now();
                break;
            }
        }
    }

} // namespace Shape::Network
