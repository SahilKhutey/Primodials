// RollbackManager.cpp
#include "RollbackManager.h"
namespace PolygonalPrimordials::LiveOps {
    std::vector<BuildVersion> RollbackManager::list_versions() const { return m_versions; }
    std::optional<BuildVersion> RollbackManager::current_version() const { return {}; }
    std::optional<BuildVersion> RollbackManager::previous_version() const { return {}; }
    bool RollbackManager::promote_to_active(const std::string&) { return true; }
    bool RollbackManager::rollback_to_previous() { return true; }
    bool RollbackManager::rollback_to(const std::string&) { return true; }
    bool RollbackManager::pin_version(const std::string&, const std::string&) { return true; }
    bool RollbackManager::unpin() { return true; }
    RollbackManager::BuildDiff RollbackManager::diff(const std::string&, const std::string&) const { return {}; }
    bool RollbackManager::start_canary(const std::string&, double) { return true; }
    bool RollbackManager::advance_canary(double) { return true; }
    bool RollbackManager::abort_canary() { return true; }
}
