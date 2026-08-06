#include "WorkshopThemeManager.h"
#include "Core/Logger.hpp"

#include <filesystem>
#include <fstream>
#include <algorithm>

namespace ShapeEngine::Steam {

    namespace fs = std::filesystem;

    WorkshopThemeManager::WorkshopThemeManager() = default;
    WorkshopThemeManager::~WorkshopThemeManager() { shutdown(); }

    bool WorkshopThemeManager::initialize(const std::string& workshopPath) {
        m_workshopPath = workshopPath;
        fs::create_directories(workshopPath);

        loadInstalledThemes();
        m_initialized = true;

        SHAPE_LOG_INFO("WorkshopThemeManager initialized: {}", workshopPath);
        return true;
    }

    void WorkshopThemeManager::shutdown() {
        m_cachedThemes.clear();
        m_subscribedIds.clear();
        m_initialized = false;
    }

    void WorkshopThemeManager::requestThemeList() {
        scanForNewThemes();
        if (m_onList) m_onList(m_cachedThemes);
    }

    bool WorkshopThemeManager::subscribe(uint64_t publishedFileId) {
        m_subscribedIds.push_back(publishedFileId);

        for (auto& item : m_cachedThemes) {
            if (item.publishedFileId == publishedFileId) {
                item.subscribed = true;
                break;
            }
        }

        std::string themeId = "workshop_" + std::to_string(publishedFileId);
        fs::create_directories(m_workshopPath + "/" + themeId);

        if (m_onInstall) m_onInstall(publishedFileId, true);
        SHAPE_LOG_INFO("Subscribed to Workshop theme: {}", publishedFileId);
        return true;
    }

    bool WorkshopThemeManager::unsubscribe(uint64_t publishedFileId) {
        auto it = std::find(m_subscribedIds.begin(), m_subscribedIds.end(), publishedFileId);
        if (it != m_subscribedIds.end()) {
            m_subscribedIds.erase(it);
        }

        for (auto& item : m_cachedThemes) {
            if (item.publishedFileId == publishedFileId) {
                item.subscribed = false;
                break;
            }
        }

        return true;
    }

    bool WorkshopThemeManager::isInstalled(uint64_t publishedFileId) const {
        std::string themeDir = m_workshopPath + "/workshop_" + std::to_string(publishedFileId);
        std::string themeJson = themeDir + "/theme.json";
        return fs::exists(themeJson);
    }

    bool WorkshopThemeManager::publishTheme(const std::string& themeId, 
                                             const std::string& title,
                                             const std::string& description) {
        (void)themeId;
        (void)title;
        (void)description;
        return true;
    }

    void WorkshopThemeManager::update() {}

    void WorkshopThemeManager::loadInstalledThemes() {
        if (!fs::exists(m_workshopPath)) return;

        for (const auto& entry : fs::directory_iterator(m_workshopPath)) {
            if (!entry.is_directory()) continue;

            std::string themeJson = entry.path().string() + "/theme.json";
            if (!fs::exists(themeJson)) continue;

            WorkshopThemeItem item;
            item.installed = true;
            m_cachedThemes.push_back(item);
        }
    }

    void WorkshopThemeManager::scanForNewThemes() {
        loadInstalledThemes();
    }

} // namespace ShapeEngine::Steam
