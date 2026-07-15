// Achievements.hpp
#pragma once

#include "Core/NonCopyable.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace PolygonalPrimordials {

    // Static definition of an achievement. Loaded from data/achievements.json
    // and/or from the Steamworks app config at startup.
    struct AchievementDef {
        std::string id;             // Internal identifier (e.g., "POPULATION_100")
        std::string api_name;       // Steam API name (e.g., "ACH_POP_100")
        std::string title;          // Display title
        std::string description;    // Player-facing description
        std::string icon_locked;     // Path to locked icon
        std::string icon_unlocked;   // Path to unlocked icon
        bool hidden = false;        // If true, hidden until unlocked
        int  progress_target = 0;   // 0 = simple unlock, >0 = progressive
    };

    // Per-player state. Persisted in cloud saves.
    struct AchievementProgress {
        std::string id;
        int current = 0;
        int target = 0;
        bool unlocked = false;

        // Unix timestamp of unlock (microseconds since epoch). 0 if locked.
        // 64 bits at microsecond resolution gives ~292,000 years of range,
        // which is comfortable.
        uint64_t unlock_time_us = 0;
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

        // Persistence
        // save_state appends to `out` a deterministic binary blob:
        //   magic[4] = "ACHV"
        //   version[4] = uint32 LE
        //   record_count[4] = uint32 LE
        //   for each record:
        //     id_len[4] = uint32 LE
        //     id[id_len] = UTF-8 bytes
        //     current[4] = int32 LE
        //     target[4] = int32 LE
        //     unlocked[1] = uint8 (0 or 1)
        //     unlock_time_us[8] = uint64 LE
        void save_state(std::vector<uint8_t>& out) const;
        bool load_state(const std::vector<uint8_t>& data);

        // Reset (for testing)
        void reset_all();

        // Diagnostics
        size_t count() const { return m_progress.size(); }
        size_t unlocked_count() const;

    private:
        Achievements() = default;

        static constexpr uint32_t SAVE_MAGIC   = 0x56434841;  // "ACHV" LE
        static constexpr uint32_t SAVE_VERSION = 1;

        // Helpers
        bool unlock_internal(const std::string& id, AchievementProgress& prog);
        AchievementProgress& ensure_progress(const std::string& id);

        std::unordered_map<std::string, AchievementDef>     m_defs;
        std::unordered_map<std::string, AchievementProgress> m_progress;
    };

} // namespace PolygonalPrimordials
