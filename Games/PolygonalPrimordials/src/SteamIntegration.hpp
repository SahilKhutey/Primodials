// SteamIntegration.h
#pragma once

#include "Core/NonCopyable.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace PolygonalPrimordials::Steam {

    struct PlayerInfo {
        uint64_t steam_id = 0;
        std::string persona_name;
        std::string country_code;
        std::string language;
        bool online = false;
        int level = 0;
    };

    struct DlcInfo {
        uint32_t app_id = 0;
        std::string name;
        bool installed = false;
        bool subscribed = false;
    };

    class SteamIntegration : public Shape::NonCopyable {
    public:
        static SteamIntegration& instance();

        bool initialize();
        void shutdown();
        bool is_initialized() const { return m_initialized; }

        // Call each frame
        void pump_callbacks();

        // User
        const PlayerInfo& player() const { return m_player; }
        bool is_online() const;
        bool is_subscribed() const;
        void request_store_url();
        void open_overlay(const std::string& dialog = "friends");

        // Stats
        void set_stat(const std::string& name, int32_t value);
        void set_stat(const std::string& name, float value);
        int32_t get_stat_int(const std::string& name) const;
        float get_stat_float(const std::string& name) const;
        bool store_stats();

        // Leaderboards
        bool upload_leaderboard_score(const std::string& board, int32_t score, bool keep_best = true);
        std::vector<int32_t> download_leaderboard_entries(const std::string& board, int count = 10);

        // DLC
        bool is_dlc_installed(uint32_t app_id) const;
        void install_dlc(uint32_t app_id);

    private:
        SteamIntegration() = default;
        bool m_initialized = false;
        PlayerInfo m_player;
    };

} // namespace
