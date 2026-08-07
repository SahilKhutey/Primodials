#pragma once

#include "Core/Platform.hpp"
#include <string>
#include <vector>

namespace Shape::Steam {

class SteamContext;

struct WorkshopItem {
    u64 publishedFileId = 0;
    std::string title;
    std::string description;
    std::string contentPath;
    bool subscribed = false;
    bool installed = false;
};

class WorkshopManager {
public:
    explicit WorkshopManager(SteamContext* context);
    ~WorkshopManager() = default;

    bool Subscribe(u64 publishedFileId);
    bool Unsubscribe(u64 publishedFileId);
    bool IsInstalled(u64 publishedFileId) const;

    const std::vector<WorkshopItem>& GetAvailableItems() const { return m_items; }
    SteamContext* GetContext() const { return m_context; }

private:
    SteamContext* m_context = nullptr;
    std::vector<WorkshopItem> m_items;
};

} // namespace Shape::Steam
