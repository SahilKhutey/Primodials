#pragma once

#include "Core/Platform.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace Shape::Steam {

class SteamContext;

enum class ConflictResolution { KeepNewest, KeepLocal, KeepRemote };

struct CloudFile {
    std::string name;
    usize size = 0;
    u64 localTimestamp = 0;
    u64 remoteTimestamp = 0;
    bool existsLocal = false;
    bool existsRemote = false;
};

class CloudSaveManager {
public:
    explicit CloudSaveManager(SteamContext* context);
    ~CloudSaveManager() = default;

    bool Upload(const std::string& filename, const std::vector<u8>& data);
    bool Download(const std::string& filename, std::vector<u8>& outData);
    bool DeleteFile(const std::string& filename);

    std::vector<CloudFile> ListFiles() const;
    SteamContext* GetContext() const { return m_context; }

private:
    SteamContext* m_context = nullptr;
    std::unordered_map<std::string, std::vector<u8>> m_cloudStorage;
};

} // namespace Shape::Steam
