// LiveOps/src/RollbackManager.h
// Manage build versions and rollback capability
#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <optional>

namespace PolygonalPrimordials::LiveOps {

    struct BuildVersion {
        std::string version;       // "1.0.1"
        std::string git_sha;      // commit hash
        uint64_t deployed_at = 0;  // unix time
        std::string channel;       // "stable", "beta", "internal"
        bool active = false;
        size_t download_count = 0;
        double crash_rate = 0;
        std::vector<std::string> notes;
    };

    class RollbackManager {
    public:
        // List
        std::vector<BuildVersion> list_versions() const;
        std::optional<BuildVersion> current_version() const;
        std::optional<BuildVersion> previous_version() const;

        // Deploy
        bool promote_to_active(const std::string& version);
        bool rollback_to_previous();
        bool rollback_to(const std::string& version);

        // Pin (force users to a specific version)
        bool pin_version(const std::string& version, const std::string& min_version = "");
        bool unpin();

        // Diff
        struct BuildDiff {
            std::vector<std::string> added_features;
            std::vector<std::string> bug_fixes;
            std::vector<std::string> breaking_changes;
            std::vector<std::string> config_changes;
        };
        BuildDiff diff(const std::string& from, const std::string& to) const;

        // Canary
        bool start_canary(const std::string& version, double percent);
        bool advance_canary(double new_percent);
        bool abort_canary();

    private:
        std::vector<BuildVersion> m_versions;
    };

} // namespace
