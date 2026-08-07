#include "Steam/Workshop/WorkshopManager.hpp"
#include "Steam/SteamContext.hpp"

namespace Shape::Steam {

WorkshopManager::WorkshopManager(SteamContext* context)
    : m_context(context) {}

bool WorkshopManager::Subscribe(u64 publishedFileId) {
    for (auto& item : m_items) {
        if (item.publishedFileId == publishedFileId) {
            item.subscribed = true;
            item.installed = true;
            return true;
        }
    }
    WorkshopItem item;
    item.publishedFileId = publishedFileId;
    item.subscribed = true;
    item.installed = true;
    m_items.push_back(item);
    return true;
}

bool WorkshopManager::Unsubscribe(u64 publishedFileId) {
    for (auto& item : m_items) {
        if (item.publishedFileId == publishedFileId) {
            item.subscribed = false;
            item.installed = false;
            return true;
        }
    }
    return false;
}

bool WorkshopManager::IsInstalled(u64 publishedFileId) const {
    for (const auto& item : m_items) {
        if (item.publishedFileId == publishedFileId) return item.installed;
    }
    return false;
}

} // namespace Shape::Steam
