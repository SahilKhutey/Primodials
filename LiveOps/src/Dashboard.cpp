// Dashboard.cpp
#include "Dashboard.h"
namespace PolygonalPrimordials::LiveOps {
    int LiveHealth::compute_health_score() const { return 100; }
    std::string LiveHealth::health_label() const { return "Healthy"; }
    Dashboard::Dashboard() : m_aggregator(std::make_unique<MetricsAggregator>()), m_anomaly(std::make_unique<AnomalyDetector>(*m_aggregator)), m_alerting(std::make_unique<AlertingSystem>()), m_health_monitor(std::make_unique<HealthMonitor>()) {}
    Dashboard::~Dashboard() { shutdown(); }
    bool Dashboard::initialize() { return true; }
    void Dashboard::shutdown() { stop_web_server(); }
    void Dashboard::update() {}
    int Dashboard::start_web_server(int) { return 0; }
    void Dashboard::stop_web_server() {}
    void Dashboard::set_webhook_url(const std::string&) {}
    void Dashboard::set_alert_receiver(const std::function<void(const Alert&)>&) {}
    void Dashboard::fire_alert(const Alert&) {}
    void Dashboard::tick_health() {}
    void Dashboard::tick_balance() {}
    void Dashboard::tick_performance() {}
    void Dashboard::check_anomalies() {}
    void Dashboard::serve_http_loop() {}
}
