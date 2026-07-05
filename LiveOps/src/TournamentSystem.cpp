// TournamentSystem.cpp
#include "TournamentSystem.h"
namespace PolygonalPrimordials::LiveOps {
    Tournament TournamentSystem::create(const std::string&, const std::string&, TournamentFormat, std::chrono::system_clock::time_point) { return {}; }
    bool TournamentSystem::open_registration(const std::string&) { return true; }
    bool TournamentSystem::start(const std::string&) { return true; }
    bool TournamentSystem::end(const std::string&) { return true; }
    bool TournamentSystem::cancel(const std::string&, const std::string&) { return true; }
    bool TournamentSystem::register_participant(const std::string&, const std::string&) { return true; }
    bool TournamentSystem::submit(const std::string&, const TournamentSubmission&) { return true; }
    bool TournamentSystem::vote(const std::string&, const std::string&, int) { return true; }
    void TournamentSystem::submit_score(const std::string&, const std::string&, double) {}
    std::vector<TournamentParticipant> TournamentSystem::leaderboard(const std::string&) const { return {}; }
    std::vector<Tournament> TournamentSystem::list() const { return {}; }
    std::vector<Tournament> TournamentSystem::active() const { return {}; }
    Tournament* TournamentSystem::get(const std::string&) { return nullptr; }
    void TournamentSystem::announce_to_discord(const std::string&) {}
    void TournamentSystem::compute_rankings(Tournament&) {}
    void TournamentSystem::award_prizes(Tournament&) {}
}
