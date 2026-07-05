// PlayerSegmentation.cpp
#include "PlayerSegmentation.h"
namespace PolygonalPrimordials::LiveOps {
    void PlayerSegmentation::add_player(const Player&) {}
    void PlayerSegmentation::update_player(const Player&) {}
    Player* PlayerSegmentation::find(const std::string&) { return nullptr; }
    const Player* PlayerSegmentation::find(const std::string&) const { return nullptr; }
    Segment PlayerSegmentation::create_segment(const std::string&, const std::string&, const std::function<bool(const Player&)>&) { return {}; }
    std::vector<Segment> PlayerSegmentation::all_segments() const { return {}; }
    std::vector<std::string> PlayerSegmentation::players_in(const std::string&) const { return {}; }
    std::vector<std::string> PlayerSegmentation::whales() const { return {}; }
    std::vector<std::string> PlayerSegmentation::dolphins() const { return {}; }
    std::vector<std::string> PlayerSegmentation::minnows() const { return {}; }
    std::vector<std::string> PlayerSegmentation::at_risk() const { return {}; }
    std::vector<std::string> PlayerSegmentation::new_players(int) const { return {}; }
    std::vector<std::string> PlayerSegmentation::tutorial_complete() const { return {}; }
    std::vector<std::string> PlayerSegmentation::first_achievement() const { return {}; }
    std::vector<std::string> PlayerSegmentation::first_civ() const { return {}; }
    std::vector<std::string> PlayerSegmentation::workshop_subscriber() const { return {}; }
    CohortMetrics PlayerSegmentation::compute_cohort(const std::string&, const std::vector<int>&) const { return {}; }
    std::vector<CohortMetrics> PlayerSegmentation::weekly_cohorts(int) const { return {}; }
    size_t PlayerSegmentation::total_players() const { return 0; }
    double PlayerSegmentation::daily_active_users() const { return 0; }
    double PlayerSegmentation::weekly_active_users() const { return 0; }
    double PlayerSegmentation::monthly_active_users() const { return 0; }
}
