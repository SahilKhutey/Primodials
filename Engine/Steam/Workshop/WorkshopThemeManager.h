#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <cstdint>

namespace ShapeEngine::Steam {

    struct WorkshopThemeItem {
        uint64_t    publishedFileId = 0;
        std::string title;
        std::string description;
        std::string previewUrl;
        std::string authorName;
        uint32_t    subscribers = 0;
        uint32_t    fileSizeKB = 0;
        bool        installed = false;
        bool        subscribed = false;
        float       rating = 0.0f;
    };

    class WorkshopThemeManager {
    public:
        WorkshopThemeManager();
        ~WorkshopThemeManager();

        bool initialize(const std::string& workshopPath);
        void shutdown();

        void requestThemeList();
        std::vector<WorkshopThemeItem> getCachedThemes() const { return m_cachedThemes; }

        bool subscribe(uint64_t publishedFileId);
        bool unsubscribe(uint64_t publishedFileId);

        bool isInstalled(uint64_t publishedFileId) const;

        void update();

        bool publishTheme(const std::string& themeId, 
                          const std::string& title,
                          const std::string& description);

        using ListCallback = std::function<void(const std::vector<WorkshopThemeItem>&)>;
        using InstallCallback = std::function<void(uint64_t itemId, bool success)>;
        void setOnList(ListCallback cb) { m_onList = std::move(cb); }
        void setOnInstall(InstallCallback cb) { m_onInstall = std::move(cb); }

        std::string getWorkshopPath() const { return m_workshopPath; }

    private:
        void loadInstalledThemes();
        void scanForNewThemes();

        std::string m_workshopPath;
        std::vector<WorkshopThemeItem> m_cachedThemes;
        std::vector<uint64_t> m_subscribedIds;

        ListCallback m_onList;
        InstallCallback m_onInstall;

        bool m_initialized = false;
    };

} // namespace ShapeEngine::Steam
