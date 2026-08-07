#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <functional>
#include <cstdint>
#include <array>

namespace ShapeEngine::Diary {

    enum class EventType : uint8_t {
        FirstSpawn,
        FirstPredation,
        FirstExtinction,
        NewSpeciesEmerged,
        PopulationBoom,
        PopulationCrash,
        LongLineage,
        CarryingCapacity,
        ColorShift,
        DailyMilestone,
        ThemeChanged,
    };

    struct DiaryEntry {
        EventType type = EventType::FirstSpawn;
        uint64_t  tickNumber = 0;
        std::chrono::system_clock::time_point wallTime;

        uint32_t speciesId = 0;
        uint32_t countBefore = 0;
        uint32_t countAfter = 0;
        uint32_t generation = 0;
        std::string description;
        std::string detailedText;
    };

    class EcosystemDiary {
    public:
        EcosystemDiary();
        ~EcosystemDiary();

        bool initialize(const std::string& diaryPath);
        void shutdown();

        void recordEvent(EventType type, const DiaryEntry& details = {});

        void notifyPopulationChange(uint32_t newCount);
        void notifySpeciesChange(uint32_t speciesId, uint32_t newCount);
        void notifyThemeChanged(const std::string& fromTheme, const std::string& toTheme);

        std::vector<DiaryEntry> getRecent(int maxEntries = 50) const;

        std::string generateSummary() const;

        bool save() const;
        bool load();

        const std::string& getPath() const { return m_path; }

        using EventCallback = std::function<void(const DiaryEntry&)>;
        void setOnEvent(EventCallback cb) { m_eventCb = std::move(cb); }

    private:
        std::string formatEntry(const DiaryEntry& entry) const;

        std::vector<DiaryEntry> m_entries;
        std::string m_path;
        size_t m_maxEntries = 1000;

        uint32_t m_lastPopulationCount = 0;
        std::array<uint32_t, 32> m_lastSpeciesCounts{};
        std::chrono::system_clock::time_point m_installTime;

        EventCallback m_eventCb;
    };

} // namespace ShapeEngine::Diary
