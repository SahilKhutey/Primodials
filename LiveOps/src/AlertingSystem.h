// LiveOps/src/AlertingSystem.h
// Multi-channel alerting (webhook, email, in-game)
#pragma once

#include "AnomalyDetector.h"
#include <string>
#include <vector>
#include <functional>
#include <chrono>

namespace PolygonalPrimordials::LiveOps {

    enum class AlertChannel : uint8_t {
        Webhook, Email, InGame, Discord, Sms
    };

    struct AlertChannelConfig {
        AlertChannel channel = AlertChannel::Webhook;
        std::string target;  // URL, email, etc.
        bool enabled = true;
        AnomalySeverity min_severity = AnomalySeverity::Warning;
    };

    struct Alert {
        std::string id;
        Anomaly anomaly;
        std::vector<AlertChannel> channels;
        std::chrono::system_clock::time_point fired_at;
        bool acknowledged = false;
        bool resolved = false;
        std::string resolved_by;
    };

    class AlertingSystem {
    public:
        void add_channel(const AlertChannelConfig& cfg);
        void remove_channel(AlertChannel ch);
        void enable_channel(AlertChannel ch, bool v);

        void fire(const Anomaly& a);

        // Manage
        void acknowledge(const std::string& alert_id);
        void resolve(const std::string& alert_id, const std::string& resolver = "");
        std::vector<Alert> open_alerts() const;
        std::vector<Alert> recent_alerts(size_t n = 50) const;

        // Hooks
        void on_alert(const std::function<void(const Alert&)>& fn);

    private:
        void send_to_channel(const Alert& a, const AlertChannelConfig& cfg);
        void send_webhook(const Alert& a, const std::string& url);
        void send_email(const Alert& a, const std::string& addr);
        void send_discord(const Alert& a, const std::string& webhook);
        void send_ingame(const Alert& a);

        std::vector<AlertChannelConfig> m_channels;
        std::vector<Alert> m_alerts;
        std::function<void(const Alert&)> m_handler;
    };

} // namespace
