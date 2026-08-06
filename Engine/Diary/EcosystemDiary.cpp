#include "EcosystemDiary.h"
#include "Core/Logger.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

namespace ShapeEngine::Diary {

    namespace fs = std::filesystem;

    EcosystemDiary::EcosystemDiary() = default;
    EcosystemDiary::~EcosystemDiary() { save(); }

    bool EcosystemDiary::initialize(const std::string& diaryPath) {
        m_path = diaryPath;
        m_installTime = std::chrono::system_clock::now();

        if (fs::exists(diaryPath)) {
            load();
        }

        SHAPE_LOG_INFO("EcosystemDiary initialized: {}", diaryPath);
        return true;
    }

    void EcosystemDiary::shutdown() {
        save();
    }

    void EcosystemDiary::recordEvent(EventType type, const DiaryEntry& details) {
        DiaryEntry entry = details;
        entry.type = type;
        entry.wallTime = std::chrono::system_clock::now();
        if (entry.description.empty()) {
            entry.description = formatEntry(entry);
        }

        m_entries.push_back(entry);

        if (m_entries.size() > m_maxEntries) {
            m_entries.erase(m_entries.begin(), 
                             m_entries.begin() + (m_entries.size() - m_maxEntries));
        }

        if (m_eventCb) m_eventCb(entry);

        SHAPE_LOG_INFO("Diary: {}", entry.description);
    }

    void EcosystemDiary::notifyPopulationChange(uint32_t newCount) {
        m_lastPopulationCount = newCount;
    }

    void EcosystemDiary::notifySpeciesChange(uint32_t speciesId, uint32_t newCount) {
        if (speciesId >= m_lastSpeciesCounts.size()) return;

        uint32_t oldCount = m_lastSpeciesCounts[speciesId];
        m_lastSpeciesCounts[speciesId] = newCount;

        if (oldCount > 0 && newCount == 0) {
            DiaryEntry entry;
            entry.speciesId = speciesId;
            entry.countBefore = oldCount;
            entry.countAfter = 0;
            entry.detailedText = "Species " + std::to_string(speciesId) + 
                                " had " + std::to_string(oldCount) + 
                                " members and is now extinct.";
            recordEvent(EventType::FirstExtinction, entry);
        } else if (oldCount == 0 && newCount > 0) {
            DiaryEntry entry;
            entry.speciesId = speciesId;
            entry.countAfter = newCount;
            entry.detailedText = "A new species (id " + std::to_string(speciesId) + 
                                ") has emerged with " + std::to_string(newCount) + 
                                " members.";
            recordEvent(EventType::NewSpeciesEmerged, entry);
        }
    }

    void EcosystemDiary::notifyThemeChanged(const std::string& from, const std::string& to) {
        DiaryEntry entry;
        entry.detailedText = "Theme changed from '" + from + "' to '" + to + "'";
        recordEvent(EventType::ThemeChanged, entry);
    }

    std::vector<DiaryEntry> EcosystemDiary::getRecent(int maxEntries) const {
        int count = std::min((int)m_entries.size(), maxEntries);
        std::vector<DiaryEntry> result;
        result.reserve(count);

        for (auto it = m_entries.rbegin(); it != m_entries.rend() && count > 0; ++it, --count) {
            result.push_back(*it);
        }
        return result;
    }

    std::string EcosystemDiary::generateSummary() const {
        std::stringstream ss;
        ss << "=== Ecosystem Diary Summary ===\n\n";
        ss << "Total events: " << m_entries.size() << "\n";
        return ss.str();
    }

    bool EcosystemDiary::save() const {
        fs::path p(m_path);
        if (p.has_parent_path() && !p.parent_path().empty()) {
            fs::create_directories(p.parent_path());
        }

        std::ofstream f(m_path);
        if (!f.is_open()) return false;

        f << "[\n";
        for (size_t i = 0; i < m_entries.size(); ++i) {
            const auto& e = m_entries[i];
            f << "  {\n";
            f << "    \"type\": " << (int)e.type << ",\n";
            f << "    \"speciesId\": " << e.speciesId << ",\n";
            f << "    \"description\": \"" << e.description << "\"\n";
            f << "  }" << (i + 1 < m_entries.size() ? "," : "") << "\n";
        }
        f << "]\n";

        return true;
    }

    bool EcosystemDiary::load() {
        std::ifstream f(m_path);
        if (!f.is_open()) return false;
        return true;
    }

    std::string EcosystemDiary::formatEntry(const DiaryEntry& entry) const {
        switch (entry.type) {
            case EventType::FirstSpawn: return "First entity spawned";
            case EventType::FirstPredation: return "First predation event";
            case EventType::FirstExtinction: return "Species " + std::to_string(entry.speciesId) + " extinct";
            case EventType::NewSpeciesEmerged: return "New species emerged";
            case EventType::PopulationBoom: return "Population boom detected";
            case EventType::PopulationCrash: return "Population crash detected";
            case EventType::LongLineage: return "Long lineage: gen " + std::to_string(entry.generation);
            case EventType::DailyMilestone: return "Daily milestone";
            case EventType::ThemeChanged: return "Theme changed";
            default: return "Event";
        }
    }

} // namespace ShapeEngine::Diary
