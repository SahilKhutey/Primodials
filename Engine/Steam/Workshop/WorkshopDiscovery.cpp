#include "WorkshopDiscovery.h"
#include "Core/Logger.hpp"

#include <algorithm>

namespace ShapeEngine::Steam {

    WorkshopDiscovery::WorkshopDiscovery(WorkshopThemeManager* manager)
        : m_manager(manager) {}

    void WorkshopDiscovery::query(const DiscoveryFilter& filter) {
        m_lastFilter = filter;
        m_currentPage = 1;
        m_current = DiscoveryResult{};
        m_queryInProgress = true;

        if (m_manager) m_manager->requestThemeList();
    }

    void WorkshopDiscovery::loadMore() {
        m_currentPage++;
        query(m_lastFilter);
    }

    void WorkshopDiscovery::update() {
        if (!m_queryInProgress || !m_manager) return;

        auto all = m_manager->getCachedThemes();
        std::vector<WorkshopThemeItem> filtered;

        for (const auto& item : all) {
            if (!m_lastFilter.searchText.empty()) {
                if (item.title.find(m_lastFilter.searchText) == std::string::npos &&
                    item.description.find(m_lastFilter.searchText) == std::string::npos) {
                    continue;
                }
            }
            if (m_lastFilter.hideSubscribed && item.subscribed) continue;
            if (m_lastFilter.hideInstalled && item.installed) continue;

            filtered.push_back(item);
        }

        m_current.themes = filtered;
        m_current.totalResults = static_cast<uint32_t>(filtered.size());
        m_current.hasMore = false;
        m_queryInProgress = false;

        if (m_onResults) m_onResults(m_current);
    }

} // namespace ShapeEngine::Steam
