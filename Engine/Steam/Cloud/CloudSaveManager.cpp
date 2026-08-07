#include "Steam/Cloud/CloudSaveManager.hpp"
#include "Steam/SteamContext.hpp"

namespace Shape::Steam {

CloudSaveManager::CloudSaveManager(SteamContext* context)
    : m_context(context) {}

bool CloudSaveManager::Upload(const std::string& filename, const std::vector<u8>& data) {
    m_cloudStorage[filename] = data;
    return true;
}

bool CloudSaveManager::Download(const std::string& filename, std::vector<u8>& outData) {
    auto it = m_cloudStorage.find(filename);
    if (it != m_cloudStorage.end()) {
        outData = it->second;
        return true;
    }
    return false;
}

bool CloudSaveManager::DeleteFile(const std::string& filename) {
    return m_cloudStorage.erase(filename) > 0;
}

std::vector<CloudFile> CloudSaveManager::ListFiles() const {
    std::vector<CloudFile> files;
    for (const auto& [name, data] : m_cloudStorage) {
        CloudFile f;
        f.name = name;
        f.size = data.size();
        f.existsLocal = true;
        f.existsRemote = true;
        files.push_back(f);
    }
    return files;
}

} // namespace Shape::Steam
