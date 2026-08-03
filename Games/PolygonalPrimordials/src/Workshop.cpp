// Workshop.cpp
#include "Workshop.hpp"
#include "SteamStub.hpp"
#include "Core/Logger.hpp"

namespace PolygonalPrimordials {

    Workshop& Workshop::instance() {
        static Workshop w;
        return w;
    }

    bool Workshop::initialize() {
        return true;
    }

    void Workshop::shutdown() {}

    void Workshop::refresh_subscribed() {}
    void Workshop::refresh_popular(int) {}
    void Workshop::search(const std::string&) {}

    bool Workshop::subscribe(uint64_t) { return true; }
    bool Workshop::unsubscribe(uint64_t) { return true; }
    bool Workshop::is_subscribed(uint64_t) const { return false; }

    bool Workshop::install(uint64_t) { return true; }
    bool Workshop::uninstall(uint64_t) { return true; }

    WorkshopItem Workshop::create_item(const std::string&, const std::string&) { return {}; }
    bool Workshop::update_item(const WorkshopItem&) { return true; }
    bool Workshop::delete_item(uint64_t) { return true; }

    std::string Workshop::get_installed_path(uint64_t) const { return ""; }

} // namespace
