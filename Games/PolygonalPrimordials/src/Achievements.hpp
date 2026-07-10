// Achievements.h
#pragma once
#include "Core/NonCopyable.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace PolygonalPrimordials {

    struct AchievementDef {
        std::string id;            // e.g. "FIRST_LAUNCH"
        std::string api_name;      // Steam API name
        std::string title;
        std::string description;
        std::string icon_locked;
        std::string icon_unlocked;
        bool hidden = false;
        int  progress_target = 0;  // 0 = simple unlock
    };

    struct AchievementProgress {
        std::string id;
        int current = 0;
        int target = 0;
        bool unlocked = false;
        uint64_t unlock_time = 0;
    };

    class Achievements : public Shape::NonCopyable {
    public:
        static Achievements& instance();

        bool initialize();
        void shutdown();

        // Definitions
        void load_definitions(const std::string& path);
        const AchievementDef* get_definition(const std::string& id) const;
        std::vector<std::string> all_ids() const;

        // Progress
        bool unlock(const std::string& id);
        bool add_progress(const std::string& id, int delta = 1);
        bool set_progress(const std::string& id, int value);
        const AchievementProgress* get_progress(const std::string& id) const;
        std::vector<AchievementProgress> all_progress() const;

        // Save/load
        void save_state(std::vector<uint8_t>& out);
        void load_state(const std::vector<uint8_t>& data);

        // Reset (for testing)
        void reset_all();

    private:
        Achievements() = default;
        std::unordered_map<std::string, AchievementDef> m_defs;
        std::unordered_map<std::string, AchievementProgress> m_progress;
    };

} // namespace
