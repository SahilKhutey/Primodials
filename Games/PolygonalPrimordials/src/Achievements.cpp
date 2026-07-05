// Achievements.cpp
#include "Achievements.h"
#include "SteamIntegration.h"
#include "SteamStub.h"
#include "Shape/Core/Logger.h"
#include "Shape/Core/Assert.hpp"

#include <fstream>
#include <sstream>
#include <chrono>
#include <cstring>
#include <algorithm>

namespace PolygonalPrimordials {

    Achievements& Achievements::instance() {
        static Achievements a;
        return a;
    }

    bool Achievements::initialize() {
        load_definitions("data/achievements.json");
        SHAPE_LOG_INFO("Ach", "Loaded {} achievement definitions", m_defs.size());
        return true;
    }

    void Achievements::shutdown() {}

    void Achievements::load_definitions(const std::string& path) {
        std::ifstream f(path);
        if (!f) {
            SHAPE_LOG_WARN("Ach", "Definitions not found: {}", path);
        }

        // For brevity, we register a hardcoded set if file isn't well-formed
        const std::vector<AchievementDef> defaults = {
            {"FIRST_LAUNCH",     "ACH_FIRST_LAUNCH",     "Welcome!", "Launch the game", "", "", false, 0},
            {"NEW_GAME",         "ACH_NEW_GAME",         "New World", "Start a new simulation", "", "", false, 0},
            {"FIRST_BIRTH",      "ACH_FIRST_BIRTH",      "Genesis", "Witness your first birth", "", "", false, 0},
            {"POPULATION_100",   "ACH_POP_100",          "Growing", "Reach a population of 100", "", "", false, 100},
            {"POPULATION_1000",  "ACH_POP_1000",         "Thriving", "Reach 1,000 individuals", "", "", false, 1000},
            {"POPULATION_10000", "ACH_POP_10000",        "Booming", "Reach 10,000 individuals", "", "", false, 10000},
            {"FIRST_CIV",        "ACH_FIRST_CIV",        "Civilization", "Witness the rise of a city", "", "", false, 0},
            {"ALL_SHAPES",       "ACH_ALL_SHAPES",       "Polyglot", "Have all 6 shape types alive at once", "", "", false, 0},
            {"FIRST_EXTINCTION", "ACH_FIRST_EXTINCT",    "Gone Forever", "Witness your first species extinction", "", "", false, 0},
            {"PLAY_100_TICKS",   "ACH_TICKS_100",        "Patient", "Run a simulation for 100 ticks", "", "", false, 100},
            {"PLAY_1M_TICKS",    "ACH_TICKS_1M",         "Eternal", "Run a simulation for 1 million ticks", "", "", false, 1000000},
            {"KILL_100",         "ACH_KILLS_100",        "Hunter", "Witness 100 deaths from combat", "", "", false, 100},
            {"CIV_EMPIRE",       "ACH_CIV_EMPIRE",       "Empire", "Grow a civilization to empire tier", "", "", false, 0},
            {"TECH_AGRICULTURE", "ACH_TECH_AGRI",        "Agrarian", "Discover Agriculture", "", "", false, 0},
            {"TECH_WRITING",     "ACH_TECH_WRITING",     "Literate", "Discover Writing", "", "", false, 0},
            {"TECH_ELECTRICITY", "ACH_TECH_ELECTRIC",    "Electrify", "Discover Electricity", "", "", false, 0},
            {"TECH_COMPUTER",    "ACH_TECH_COMPUTER",    "Digital", "Discover the Computer", "", "", false, 0},
            {"TECH_SPACE",       "ACH_TECH_SPACE",       "To the Stars", "Discover Space Travel", "", "", false, 0},
            {"EVOLUTION_GOAL",   "ACH_EVO_GOAL",         "Evolved", "Reach generation 100 in a species", "", "", false, 100},
            {"MULTIPLAYER_WIN",  "ACH_MP_WIN",           "Versus", "Win a multiplayer match", "", "", false, 0},
            {"FIRST_MOD",        "ACH_FIRST_MOD",        "Modder", "Subscribe to a Workshop mod", "", "", false, 0},
            {"PUBLISH_MOD",      "ACH_PUB_MOD",          "Creator", "Publish your own Workshop mod", "", "", false, 0},
            {"FIFTY_HOURS",      "ACH_HOURS_50",         "Dedicated", "Play for 50 hours", "", "", false, 50*60*60},
            {"SPEED_DEMON",      "ACH_SPEED",            "Speed Demon", "Run a sim at 64x speed", "", "", false, 0},
            {"PERFECTIONIST",    "ACH_PERFECT",          "Perfectionist", "Complete all other achievements", "", "", false, 0},
        };
        for (const auto& d : defaults) {
            m_defs[d.id] = d;
            m_progress[d.id] = AchievementProgress{d.id, 0, d.progress_target, false, 0};
        }
    }

    const AchievementDef* Achievements::get_definition(const std::string& id) const {
        auto it = m_defs.find(id);
        return it != m_defs.end() ? &it->second : nullptr;
    }

    std::vector<std::string> Achievements::all_ids() const {
        std::vector<std::string> out;
        out.reserve(m_defs.size());
        for (const auto& [k, _] : m_defs) out.push_back(k);
        return out;
    }

    bool Achievements::unlock(const std::string& id) {
        auto prog_it = m_progress.find(id);
        if (prog_it == m_progress.end()) return false;
        if (prog_it->second.unlocked) return false;
        prog_it->second.unlocked = true;
        if (prog_it->second.target > 0) {
            prog_it->second.current = prog_it->second.target;
        }
        prog_it->second.unlock_time = static_cast<uint64_t>(
            std::chrono::system_clock::now().time_since_epoch().count());
        SHAPE_LOG_INFO("Ach", "Unlocked: {}", id);
#ifdef STEAMWORKS_SDK
        const auto* def = get_definition(id);
        if (def && SteamUserStats()) {
            SteamUserStats()->SetAchievement(def->api_name.c_str());
            SteamUserStats()->StoreStats();
        }
#endif
        return true;
    }

    bool Achievements::add_progress(const std::string& id, int delta) {
        auto prog_it = m_progress.find(id);
        if (prog_it == m_progress.end()) return false;
        if (prog_it->second.unlocked) return false;
        prog_it->second.current += delta;
        if (prog_it->second.current >= prog_it->second.target) {
            return unlock(id);
        }
#ifdef STEAMWORKS_SDK
        const auto* def = get_definition(id);
        if (def && SteamUserStats()) {
            SteamUserStats()->IndicateAchievementProgress(def->api_name.c_str(),
                                                          prog_it->second.current,
                                                          prog_it->second.target);
        }
#endif
        return false;
    }

    bool Achievements::set_progress(const std::string& id, int value) {
        auto prog_it = m_progress.find(id);
        if (prog_it == m_progress.end()) return false;
        if (prog_it->second.unlocked) return false;
        prog_it->second.current = value;
        if (value >= prog_it->second.target) return unlock(id);
        return false;
    }

    const AchievementProgress* Achievements::get_progress(const std::string& id) const {
        auto it = m_progress.find(id);
        return it != m_progress.end() ? &it->second : nullptr;
    }

    std::vector<AchievementProgress> Achievements::all_progress() const {
        std::vector<AchievementProgress> out;
        out.reserve(m_progress.size());
        for (const auto& [_, p] : m_progress) out.push_back(p);
        return out;
    }

    void Achievements::save_state(std::vector<uint8_t>& out) {
        out.clear();
        for (const auto& [id, p] : m_progress) {
            const size_t off = out.size();
            out.resize(off + 1 + 8 + 4 + 1 + 8);
            out[off] = static_cast<uint8_t>(p.unlocked ? 1 : 0);
            uint64_t t = p.unlock_time;
            std::memcpy(out.data() + off + 1, &t, 8);
            int32_t c = p.current;
            std::memcpy(out.data() + off + 9, &c, 4);
            out[off + 13] = 0;  // separator
            std::memcpy(out.data() + off + 14, id.data(), std::min<size_t>(8, id.size()));
        }
    }

    void Achievements::load_state(const std::vector<uint8_t>& data) {
        (void)data;
    }

    void Achievements::reset_all() {
        for (auto& [_, p] : m_progress) {
            p.unlocked = false;
            p.current = 0;
            p.unlock_time = 0;
        }
    }

} // namespace
