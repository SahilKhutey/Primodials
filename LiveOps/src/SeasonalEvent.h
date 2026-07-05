// LiveOps/src/SeasonalEvent.h
// Seasonal events and limited-time content
#pragma once

#include <string>
#include <vector>
#include <chrono>

namespace PolygonalPrimordials::LiveOps {

    enum class EventType : uint8_t {
        Holiday, Anniversary, Community, Lore, Special
    };

    struct SeasonalEvent {
        std::string id;
        std::string name;
        std::string description;
        EventType type = EventType::Special;
        std::chrono::system_clock::time_point starts_at;
        std::chrono::system_clock::time_point ends_at;
        bool active = false;
        std::vector<std::string> affected_species;     // new species added
        std::vector<std::string> affected_achievements; // new achievements
        std::vector<std::string> affected_tech;         // new tech
        std::string discord_event_url;
        std::string steam_event_id;
        int participation_count = 0;
    };

    class SeasonalEventSystem {
    public:
        SeasonalEvent create(const std::string& name, EventType type,
                              std::chrono::system_clock::time_point start,
                              std::chrono::system_clock::time_point end);
        bool start_event(const std::string& id);
        bool end_event(const std::string& id);
        bool cancel_event(const std::string& id);

        // Active
        SeasonalEvent* current_event();
        std::vector<SeasonalEvent> active() const;
        std::vector<SeasonalEvent> upcoming(int days_ahead = 30) const;
        std::vector<SeasonalEvent> past(int n = 10) const;

        // Recurring
        void schedule_recurring(const std::string& name, EventType type,
                                 int day_of_year, int duration_days);

        // Stats
        int total_participants() const;

    private:
        std::vector<SeasonalEvent> m_events;
    };

} // namespace
