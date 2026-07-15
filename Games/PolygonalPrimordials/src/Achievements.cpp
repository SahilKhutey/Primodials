// Achievements.cpp
#include "Achievements.hpp"
#include "SteamIntegration.hpp"
#include "SteamStub.hpp"
#include "Core/Logger.hpp"
#include "Core/Assert.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>

namespace PolygonalPrimordials {

    // ============================================================
    // Singleton
    // ============================================================

    Achievements& Achievements::instance() {
        static Achievements a;
        return a;
    }

    // ============================================================
    // Lifecycle
    // ============================================================

    bool Achievements::initialize() {
        load_definitions("data/achievements.json");
        SHAPE_LOG_INFO("Achievements: Loaded {} achievement definitions", m_defs.size());
        return true;
    }

    void Achievements::shutdown() {
        m_defs.clear();
        m_progress.clear();
    }

    // ============================================================
    // Definitions
    // ============================================================

    void Achievements::load_definitions(const std::string& path) {
        // First, register the hardcoded defaults. These are always
        // available even if the JSON file is missing.
        const struct {
            const char* id;
            const char* api_name;
            const char* title;
            const char* description;
            int  progress_target;
        } defaults[] = {
            {"FIRST_LAUNCH",     "ACH_FIRST_LAUNCH",     "Welcome!",           "Launch the game",                          0},
            {"NEW_GAME",         "ACH_NEW_GAME",         "New World",          "Start a new simulation",                    0},
            {"FIRST_BIRTH",      "ACH_FIRST_BIRTH",      "Genesis",            "Witness your first birth",                 0},
            {"POPULATION_100",   "ACH_POP_100",          "Growing",            "Reach a population of 100",                100},
            {"POPULATION_1000",  "ACH_POP_1000",         "Thriving",           "Reach 1,000 individuals",                 1000},
            {"POPULATION_10000", "ACH_POP_10000",        "Booming",            "Reach 10,000 individuals",               10000},
            {"FIRST_CIV",        "ACH_FIRST_CIV",        "Civilization",       "Witness the rise of a city",                0},
            {"ALL_SHAPES",       "ACH_ALL_SHAPES",       "Polyglot",           "Have all 6 shape types alive at once",      0},
            {"FIRST_EXTINCTION", "ACH_FIRST_EXTINCT",    "Gone Forever",       "Witness your first species extinction",     0},
            {"PLAY_100_TICKS",   "ACH_TICKS_100",        "Patient",            "Run a simulation for 100 ticks",          100},
            {"PLAY_1M_TICKS",    "ACH_TICKS_1M",         "Eternal",            "Run a simulation for 1 million ticks",1000000},
            {"KILL_100",         "ACH_KILLS_100",        "Hunter",             "Witness 100 deaths from combat",            100},
            {"CIV_EMPIRE",       "ACH_CIV_EMPIRE",       "Empire",             "Grow a civilization to empire tier",         0},
            {"TECH_AGRICULTURE", "ACH_TECH_AGRI",        "Agrarian",           "Discover Agriculture",                      0},
            {"TECH_WRITING",     "ACH_TECH_WRITING",     "Literate",           "Discover Writing",                          0},
            {"TECH_ELECTRICITY", "ACH_TECH_ELECTRIC",    "Electrify",          "Discover Electricity",                      0},
            {"TECH_COMPUTER",    "ACH_TECH_COMPUTER",    "Digital",            "Discover the Computer",                     0},
            {"TECH_SPACE",       "ACH_TECH_SPACE",       "To the Stars",       "Discover Space Travel",                     0},
            {"EVOLUTION_GOAL",   "ACH_EVO_GOAL",         "Evolved",            "Reach generation 100 in a species",        100},
            {"MULTIPLAYER_WIN",  "ACH_MP_WIN",           "Versus",             "Win a multiplayer match",                   0},
            {"FIRST_MOD",        "ACH_FIRST_MOD",        "Modder",             "Subscribe to a Workshop mod",               0},
            {"PUBLISH_MOD",      "ACH_PUB_MOD",          "Creator",            "Publish your own Workshop mod",             0},
            {"FIFTY_HOURS",      "ACH_HOURS_50",         "Dedicated",          "Play for 50 hours",                  50*60*60},
            {"SPEED_DEMON",      "ACH_SPEED",            "Speed Demon",        "Run a sim at 64x speed",                    0},
            {"PERFECTIONIST",    "ACH_PERFECT",          "Perfectionist",      "Complete all other achievements",           0},
        };

        for (const auto& d : defaults) {
            AchievementDef def;
            def.id              = d.id;
            def.api_name        = d.api_name;
            def.title           = d.title;
            def.description     = d.description;
            def.progress_target = d.progress_target;
            m_defs[d.id] = def;
            // Initialize progress to match.
            m_progress[d.id] = AchievementProgress{d.id, 0, d.progress_target, false, 0};
        }

        // If a JSON file exists, parse it and override titles/descriptions.
        std::ifstream f(path);
        if (!f) {
            SHAPE_LOG_WARN("Achievements: Definitions file not found: {} (using built-in defaults)", path);
            return;
        }
        std::string content((std::istreambuf_iterator<char>(f)),
                            std::istreambuf_iterator<char>());

        // Minimal JSON parsing — extract title and description per record.
        size_t pos = 0;
        int overridden = 0;
        while (pos < content.size()) {
            const auto id_pos = content.find("\"id\":", pos);
            if (id_pos == std::string::npos) break;

            // Extract id value
            size_t p = content.find('"', id_pos + 5);
            if (p == std::string::npos) break;
            const size_t id_start = p + 1;
            const size_t id_end = content.find('"', id_start);
            if (id_end == std::string::npos) break;
            const std::string id = content.substr(id_start, id_end - id_start);

            // Find title
            const auto title_pos = content.find("\"title\":", id_end);
            if (title_pos != std::string::npos) {
                size_t t = content.find('"', title_pos + 8);
                if (t != std::string::npos) {
                    const size_t title_end = content.find('"', t + 1);
                    if (title_end != std::string::npos) {
                        const std::string title = content.substr(t + 1, title_end - t - 1);
                        auto it = m_defs.find(id);
                        if (it != m_defs.end()) {
                            it->second.title = title;
                            ++overridden;
                        }
                    }
                }
            }

            pos = id_end + 1;
        }

        SHAPE_LOG_INFO("Achievements: JSON overrides applied: {} (out of {} definitions)",
                       overridden, m_defs.size());
    }

    const AchievementDef* Achievements::get_definition(const std::string& id) const {
        auto it = m_defs.find(id);
        return it != m_defs.end() ? &it->second : nullptr;
    }

    std::vector<std::string> Achievements::all_ids() const {
        std::vector<std::string> out;
        out.reserve(m_defs.size());
        for (const auto& kv : m_defs) out.push_back(kv.first);
        return out;
    }

    // ============================================================
    // Progress
    // ============================================================

    AchievementProgress& Achievements::ensure_progress(const std::string& id) {
        auto [it, inserted] = m_progress.try_emplace(id);
        if (inserted) {
            it->second.id = id;
            it->second.target = 0;
            it->second.current = 0;
            it->second.unlocked = false;
            it->second.unlock_time_us = 0;
            SHAPE_LOG_WARN("Achievements: Created on-the-fly progress for unknown ID '{}'.", id);
        }
        return it->second;
    }

    bool Achievements::unlock_internal(const std::string& id, AchievementProgress& prog) {
        if (prog.unlocked) return false;

        prog.unlocked = true;
        if (prog.target > 0) {
            prog.current = prog.target;
        }
        using namespace std::chrono;
        prog.unlock_time_us = static_cast<uint64_t>(
            duration_cast<microseconds>(system_clock::now().time_since_epoch()).count());

        SHAPE_LOG_INFO("Achievements: Unlocked: {} (at t={}us)", id, prog.unlock_time_us);

#ifdef STEAMWORKS_SDK
        const auto* def = get_definition(id);
        if (def && SteamUserStats()) {
            SteamUserStats()->SetAchievement(def->api_name.c_str());
            SteamUserStats()->StoreStats();
        }
#endif
        return true;
    }

    bool Achievements::unlock(const std::string& id) {
        auto it = m_progress.find(id);
        if (it == m_progress.end()) {
            SHAPE_LOG_WARN("Achievements: unlock() called for unknown ID '{}'", id);
            return false;
        }
        return unlock_internal(id, it->second);
    }

    bool Achievements::add_progress(const std::string& id, int delta) {
        AchievementProgress& prog = ensure_progress(id);
        if (prog.unlocked) return false;
        if (delta <= 0) return false;

        prog.current += delta;
        if (prog.current >= prog.target && prog.target > 0) {
            return unlock_internal(id, prog);
        }

#ifdef STEAMWORKS_SDK
        const auto* def = get_definition(id);
        if (def && SteamUserStats()) {
            SteamUserStats()->IndicateAchievementProgress(
                def->api_name.c_str(),
                static_cast<uint32_t>(prog.current),
                static_cast<uint32_t>(prog.target));
        }
#endif
        return false;
    }

    bool Achievements::set_progress(const std::string& id, int value) {
        auto it = m_progress.find(id);
        if (it == m_progress.end()) {
            SHAPE_LOG_WARN("Achievements: set_progress() called for unknown ID '{}'", id);
            return false;
        }
        if (it->second.unlocked) return false;
        if (value < 0) return false;

        it->second.current = value;
        if (value >= it->second.target && it->second.target > 0) {
            return unlock_internal(id, it->second);
        }
        return false;
    }

    const AchievementProgress* Achievements::get_progress(const std::string& id) const {
        auto it = m_progress.find(id);
        return it != m_progress.end() ? &it->second : nullptr;
    }

    std::vector<AchievementProgress> Achievements::all_progress() const {
        std::vector<AchievementProgress> out;
        out.reserve(m_progress.size());
        for (const auto& kv : m_progress) out.push_back(kv.second);
        return out;
    }

    size_t Achievements::unlocked_count() const {
        size_t n = 0;
        for (const auto& kv : m_progress) {
            if (kv.second.unlocked) ++n;
        }
        return n;
    }

    // ============================================================
    // Persistence
    // ============================================================

    void Achievements::save_state(std::vector<uint8_t>& out) const {
        // Record starting offset so we can patch the count after writing.
        const size_t header_offset = out.size();
        const size_t body_offset = header_offset + 12;

        // Resize for header; we'll fill body in below.
        out.resize(body_offset);

        // Magic
        const uint32_t magic   = SAVE_MAGIC;
        const uint32_t version = SAVE_VERSION;
        std::memcpy(out.data() + header_offset + 0, &magic,   4);
        std::memcpy(out.data() + header_offset + 4, &version, 4);

        // Write each record.
        uint32_t record_count = 0;
        for (const auto& kv : m_progress) {
            const std::string& id = kv.first;
            const AchievementProgress& p = kv.second;

            const uint32_t id_len = static_cast<uint32_t>(id.size());
            const size_t rec_size = 4 + id_len + 4 + 4 + 1 + 8;
            const size_t off = out.size();
            out.resize(off + rec_size);

            std::memcpy(out.data() + off + 0, &id_len, 4);
            std::memcpy(out.data() + off + 4, id.data(), id_len);
            std::memcpy(out.data() + off + 4 + id_len, &p.current, 4);
            std::memcpy(out.data() + off + 4 + id_len + 4, &p.target, 4);
            out[off + 4 + id_len + 8] = p.unlocked ? 1 : 0;
            std::memcpy(out.data() + off + 4 + id_len + 9, &p.unlock_time_us, 8);
            ++record_count;
        }

        std::memcpy(out.data() + header_offset + 8, &record_count, 4);
    }

    bool Achievements::load_state(const std::vector<uint8_t>& data) {
        if (data.size() < 12) {
            SHAPE_LOG_WARN("Achievements: load_state: data too small ({} bytes)", data.size());
            return false;
        }

        uint32_t magic = 0, version = 0, record_count = 0;
        std::memcpy(&magic,        data.data() + 0, 4);
        std::memcpy(&version,      data.data() + 4, 4);
        std::memcpy(&record_count, data.data() + 8, 4);

        if (magic != SAVE_MAGIC) {
            SHAPE_LOG_WARN("Achievements: load_state: bad magic 0x{:08X}", magic);
            return false;
        }
        if (version != SAVE_VERSION) {
            SHAPE_LOG_WARN("Achievements: load_state: unsupported version {}", version);
            return false;
        }

        size_t off = 12;
        int restored = 0;
        for (uint32_t i = 0; i < record_count; ++i) {
            if (off + 4 > data.size()) {
                SHAPE_LOG_WARN("Achievements: load_state: truncated at record {}", i);
                return false;
            }
            uint32_t id_len = 0;
            std::memcpy(&id_len, data.data() + off, 4);
            off += 4;

            if (off + id_len > data.size()) {
                SHAPE_LOG_WARN("Achievements: load_state: ID truncated at record {}", i);
                return false;
            }
            std::string id(reinterpret_cast<const char*>(data.data() + off), id_len);
            off += id_len;

            if (off + 17 > data.size()) {
                SHAPE_LOG_WARN("Achievements: load_state: body truncated at record {}", i);
                return false;
            }
            int32_t current = 0, target = 0;
            uint8_t unlocked = 0;
            uint64_t unlock_time_us = 0;
            std::memcpy(&current,         data.data() + off,     4);
            std::memcpy(&target,          data.data() + off + 4, 4);
            std::memcpy(&unlocked,        data.data() + off + 8, 1);
            std::memcpy(&unlock_time_us,  data.data() + off + 9, 8);
            off += 17;

            AchievementProgress& p = ensure_progress(id);
            p.current = current;
            p.target = std::max(p.target, target);
            p.unlocked = (unlocked != 0);
            p.unlock_time_us = unlock_time_us;
            ++restored;
        }

        SHAPE_LOG_INFO("Achievements: load_state: restored {} progress records", restored);
        return true;
    }

    // ============================================================
    // Reset
    // ============================================================

    void Achievements::reset_all() {
        for (auto& entry : m_progress) {
            AchievementProgress& p = entry.second;
            p.current = 0;
            p.unlocked = false;
            p.unlock_time_us = 0;
        }
    }

} // namespace PolygonalPrimordials
