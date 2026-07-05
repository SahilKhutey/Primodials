// LiveOps/src/TournamentSystem.h
// In-game and Discord-linked tournaments
#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <cstdint>

namespace PolygonalPrimordials::LiveOps {

    enum class TournamentFormat : uint8_t {
        SpeedRun,         // Fastest to reach X
        Survival,         // Highest population at Y hours
        Civilization,     // Largest empire
        Combat,           // Most KOs
        Creative,         // Most beautiful world
        Modding,          // Best new mod
        Screenshot        // Best screenshot
    };

    enum class TournamentState : uint8_t {
        Scheduled, Registration, Running, Judging, Completed, Cancelled
    };

    struct TournamentPrize {
        std::string name;
        std::string description;
        int xp = 0;
        std::string badge_url;
        int steam_achievement_id = 0;
        int value_usd = 0;
    };

    struct TournamentParticipant {
        std::string user_id;
        std::string display_name;
        std::chrono::system_clock::time_point registered_at;
        double score = 0;
        int rank = 0;
    };

    struct TournamentSubmission {
        std::string user_id;
        std::string title;
        std::string description;
        std::vector<std::string> media_urls;
        std::chrono::system_clock::time_point submitted_at;
        int votes = 0;
    };

    struct Tournament {
        std::string id;
        std::string name;
        std::string description;
        TournamentFormat format;
        TournamentState state = TournamentState::Scheduled;
        std::chrono::system_clock::time_point registration_opens;
        std::chrono::system_clock::time_point starts_at;
        std::chrono::system_clock::time_point ends_at;
        int max_participants = 0;
        int current_participants = 0;
        std::vector<std::string> rules;
        std::vector<TournamentPrize> prizes;
        std::vector<TournamentParticipant> participants;
        std::vector<TournamentSubmission> submissions;
    };

    class TournamentSystem {
    public:
        Tournament create(const std::string& name, const std::string& description,
                          TournamentFormat format, std::chrono::system_clock::time_point start);
        bool open_registration(const std::string& id);
        bool start(const std::string& id);
        bool end(const std::string& id);
        bool cancel(const std::string& id, const std::string& reason);

        // Register
        bool register_participant(const std::string& tournament_id, const std::string& user_id);
        bool submit(const std::string& tournament_id, const TournamentSubmission& sub);
        bool vote(const std::string& tournament_id, const std::string& submission_user, int votes = 1);

        // Score
        void submit_score(const std::string& tournament_id, const std::string& user_id, double score);
        std::vector<TournamentParticipant> leaderboard(const std::string& tournament_id) const;

        // List
        std::vector<Tournament> list() const;
        std::vector<Tournament> active() const;
        Tournament* get(const std::string& id);

        // Discord integration
        void announce_to_discord(const std::string& tournament_id);

    private:
        void compute_rankings(Tournament& t);
        void award_prizes(Tournament& t);

        std::vector<Tournament> m_tournaments;
    };

} // namespace
