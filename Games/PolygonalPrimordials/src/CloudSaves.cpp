// CloudSaves.cpp
#include "CloudSaves.h"
#include "SteamStub.h"
#include "Shape/Core/Logger.h"
#include "Shape/Core/Platform.h"

#include <filesystem>
#include <fstream>

namespace PolygonalPrimordials {

    CloudSaves& CloudSaves::instance() {
        static CloudSaves c;
        return c;
    }

    bool CloudSaves::initialize() {
        m_initialized = true;
        Shape::Platform::create_directory("Saves");
        return true;
    }

    void CloudSaves::shutdown() {
        m_initialized = false;
    }

    bool CloudSaves::save(const std::string& slot, const std::vector<uint8_t>& data) {
        const std::string path = "Saves/" + slot + ".shape";
        std::ofstream f(path, std::ios::binary);
        if (!f) return false;
        f.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
        return f.good();
    }

    std::vector<uint8_t> CloudSaves::load(const std::string& slot) {
        const std::string path = "Saves/" + slot + ".shape";
        std::ifstream f(path, std::ios::binary);
        if (!f) return {};
        return std::vector<uint8_t>((std::istreambuf_iterator<char>(f)),
                                    std::istreambuf_iterator<char>());
    }

    bool CloudSaves::exists(const std::string& slot) const {
        return Shape::Platform::file_exists("Saves/" + slot + ".shape");
    }

    bool CloudSaves::remove(const std::string& slot) {
        return Shape::Platform::delete_file("Saves/" + slot + ".shape");
    }

    void CloudSaves::sync_now() {
        m_syncing = true;
        // In real impl: upload local files to Steam Cloud
        m_syncing = false;
    }

    bool CloudSaves::is_syncing() const { return m_syncing; }

    size_t CloudSaves::quota_bytes() const {
#ifdef STEAMWORKS_SDK
        if (SteamRemoteStorage()) return SteamRemoteStorage()->GetQuota() * 1024;
#endif
        return 100 * 1024 * 1024;
    }

    size_t CloudSaves::used_bytes() const {
#ifdef STEAMWORKS_SDK
        if (SteamRemoteStorage()) {
            return SteamRemoteStorage()->GetQuota() - SteamRemoteStorage()->GetQuotaAvailable() * 1024;
        }
#endif
        return 0;
    }

} // namespace
