// SeasonalEvent.cpp
#include "SeasonalEvent.h"
namespace PolygonalPrimordials::LiveOps {
    SeasonalEvent SeasonalEventSystem::create(const std::string&, EventType, std::chrono::system_clock::time_point, std::chrono::system_clock::time_point) { return {}; }
    bool SeasonalEventSystem::start_event(const std::string&) { return true; }
    bool SeasonalEventSystem::end_event(const std::string&) { return true; }
    bool SeasonalEventSystem::cancel_event(const std::string&) { return true; }
    SeasonalEvent* SeasonalEventSystem::current_event() { return nullptr; }
    std::vector<SeasonalEvent> SeasonalEventSystem::active() const { return {}; }
    std::vector<SeasonalEvent> SeasonalEventSystem::upcoming(int) const { return {}; }
    std::vector<SeasonalEvent> SeasonalEventSystem::past(int) const { return {}; }
    void SeasonalEventSystem::schedule_recurring(const std::string&, EventType, int, int) {}
    int SeasonalEventSystem::total_participants() const { return 0; }
}
