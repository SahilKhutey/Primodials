// CloudSaves.h
#pragma once
#include "Core/NonCopyable.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace PolygonalPrimordials {

    class CloudSaves : public Shape::NonCopyable {
    public:
        static CloudSaves& instance();

        bool initialize();
        void shutdown();

        // Save/load
        bool save(const std::string& slot, const std::vector<uint8_t>& data);
        std::vector<uint8_t> load(const std::string& slot);
        bool exists(const std::string& slot) const;
        bool remove(const std::string& slot);

        // Sync
        void sync_now();
        bool is_syncing() const;

        // Quota
        size_t quota_bytes() const;
        size_t used_bytes() const;

    private:
        CloudSaves() = default;
        bool m_initialized = false;
        bool m_syncing = false;
    };

} // namespace
