#pragma once

#include "WorkshopThemeManager.h"
#include <string>
#include <vector>
#include <functional>

namespace ShapeEngine::Steam {

    enum class DiscoverySort {
        Trending,
        TopRated,
        MostSubscribed,
        Newest,
        RecentlyUpdated,
    };

    enum class DiscoveryTimeframe {
        Today,
        ThisWeek,
        ThisMonth,
        AllTime
    };

    struct DiscoveryFilter {
        DiscoverySort sort = DiscoverySort::Trending;
        DiscoveryTimeframe timeframe = DiscoveryTimeframe::ThisWeek;
        std::vector<std::string> tags;
        std::string searchText;
        bool hideSubscribed = false;
        bool hideInstalled = false;
    };

    struct DiscoveryResult {
        std::vector<WorkshopThemeItem> themes;
        bool hasMore = false;
        uint32_t totalResults = 0;
    };

    class WorkshopDiscovery {
    public:
        explicit WorkshopDiscovery(WorkshopThemeManager* manager);

        void query(const DiscoveryFilter& filter);
        void loadMore();

        const DiscoveryResult& getCurrentResults() const { return m_current; }

        using ResultsCallback = std::function<void(const DiscoveryResult&)>;
        void setOnResults(ResultsCallback cb) { m_onResults = std::move(cb); }

        void update();

    private:
        WorkshopThemeManager* m_manager;
        DiscoveryResult m_current;
        DiscoveryFilter m_lastFilter;
        int m_currentPage = 1;
        bool m_queryInProgress = false;
        ResultsCallback m_onResults;
    };

} // namespace ShapeEngine::Steam
