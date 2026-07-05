// RemoteConfig.cpp
#include "RemoteConfig.h"
namespace PolygonalPrimordials::LiveOps {
    ConfigValue RemoteConfig::get(const std::string&) const { return {}; }
    bool RemoteConfig::set(const std::string&, ConfigValue, const std::string&) { return true; }
    bool RemoteConfig::reset(const std::string&) { return true; }
    int64_t RemoteConfig::get_int(const std::string&, int64_t def) const { return def; }
    double RemoteConfig::get_float(const std::string&, double def) const { return def; }
    bool RemoteConfig::get_bool(const std::string&, bool def) const { return def; }
    std::string RemoteConfig::get_string(const std::string&, const std::string& def) const { return def; }
    std::unordered_map<std::string, ConfigValue> RemoteConfig::snapshot() const { return {}; }
    bool RemoteConfig::apply(const std::unordered_map<std::string, ConfigValue>&, const std::string&) { return true; }
    std::vector<RemoteConfig::HistoryEntry> RemoteConfig::history() const { return {}; }
    std::vector<RemoteConfig::HistoryEntry> RemoteConfig::history(const std::string&) const { return {}; }
    void RemoteConfig::pull() {}
    void RemoteConfig::push() {}
}
