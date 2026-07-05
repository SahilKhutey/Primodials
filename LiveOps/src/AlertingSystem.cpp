// AlertingSystem.cpp
#include "AlertingSystem.h"
namespace PolygonalPrimordials::LiveOps {
    void AlertingSystem::add_channel(const AlertChannelConfig&) {}
    void AlertingSystem::remove_channel(AlertChannel) {}
    void AlertingSystem::enable_channel(AlertChannel, bool) {}
    void AlertingSystem::fire(const Anomaly&) {}
    void AlertingSystem::acknowledge(const std::string&) {}
    void AlertingSystem::resolve(const std::string&, const std::string&) {}
    std::vector<Alert> AlertingSystem::open_alerts() const { return {}; }
    std::vector<Alert> AlertingSystem::recent_alerts(size_t) const { return {}; }
    void AlertingSystem::on_alert(const std::function<void(const Alert&)>&) {}
    void AlertingSystem::send_to_channel(const Alert&, const AlertChannelConfig&) {}
    void AlertingSystem::send_webhook(const Alert&, const std::string&) {}
    void AlertingSystem::send_email(const Alert&, const std::string&) {}
    void AlertingSystem::send_discord(const Alert&, const std::string&) {}
    void AlertingSystem::send_ingame(const Alert&) {}
}
