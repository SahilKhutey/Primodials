// LiveOps/src/Dashboard.h
// Real-time metrics dashboard for live operations
#pragma once

#include "Shape/Core/Time.h"
#include "MetricsAggregator.h"
#include "AnomalyDetector.h"
#include "AlertingSystem.h"
#include "HealthMonitor.h"

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cstdint>
#include <thread>
#include <atomic>

namespace PolygonalPrimordials::LiveOps {

    // High-level health metrics
    struct LiveHealth {
        double crash_rate_per_hour = 0;     // Crashes / hours played
        double avg_session_length = 0;     // Minutes
        double retention_d1 = 0;           // % returning day 1
        double retention_d7 = 0;           // % returning day 7
        double retention_d30 = 0;          // % returning day 30
        int active_players_now = 0;
        int peak_today = 0;
        int achievements_unlocked_per_min = 0;
        int workshop_subs_per_hour = 0;
        double avg_fps = 0;
        double p99_fps = 0;                 // 99th percentile
        int error_count_last_hour = 0;
        int support_tickets_open = 0;
        int support_tickets_resolved_24h = 0;

        // Computed health score (0-100)
        int compute_health_score() const;
        std::string health_label() const;  // "Healthy" | "Degraded" | "Critical"
    };

    // Game balance metrics (live)
    struct BalanceMetrics {
        float avg_creature_lifespan = 0;       // ticks
        float avg_population_growth_rate = 0;  // per hour
        std::vector<std::pair<int, float>> species_population;  // [id, count]
        std::vector<std::pair<int, float>> species_growth;      // [id, rate]
        float combat_encounter_rate = 0;
        float reproduction_rate = 0;
        float extinction_rate_per_day = 0;
        float speciation_rate_per_day = 0;
        float tech_discovery_rate_per_day = 0;
    };

    // Performance metrics
    struct PerformanceMetrics {
        double avg_frame_ms = 0;
        double p99_frame_ms = 0;
        double worst_frame_ms = 0;
        int entity_count_p50 = 0;
        int entity_count_p99 = 0;
        int memory_p50_mb = 0;
        int memory_p99_mb = 0;
        std::vector<double> ms_per_system;  // per-system breakdown
    };

    class Dashboard {
    public:
        Dashboard();
        ~Dashboard();

        bool initialize();
        void shutdown();
        void update();  // Called every 60s

        // Access
        const LiveHealth& health() const { return m_health; }
        const BalanceMetrics& balance() const { return m_balance; }
        const PerformanceMetrics& performance() const { return m_performance; }

        // Server
        int start_web_server(int port = 8080);
        void stop_web_server();

        // Webhook
        void set_webhook_url(const std::string& url);
        void set_alert_receiver(const std::function<void(const Alert&)>& receiver);

        // Manual alerts
        void fire_alert(const Alert& a);

        // Streaming
        const MetricsAggregator& aggregator() const { return *m_aggregator; }
        AnomalyDetector& anomaly() { return *m_anomaly; }
        AlertingSystem& alerting() { return *m_alerting; }

    private:
        void tick_health();
        void tick_balance();
        void tick_performance();
        void check_anomalies();
        void serve_http_loop();

        std::unique_ptr<MetricsAggregator> m_aggregator;
        std::unique_ptr<AnomalyDetector> m_anomaly;
        std::unique_ptr<AlertingSystem> m_alerting;
        std::unique_ptr<HealthMonitor> m_health_monitor;

        LiveHealth m_health;
        BalanceMetrics m_balance;
        PerformanceMetrics m_performance;

        std::string m_webhook_url;
        std::function<void(const Alert&)> m_alert_receiver;

        std::thread m_http_thread;
        std::atomic<bool> m_running{false};
        int m_http_port = 8080;

        Shape::Time::TimePoint m_last_update;
    };

} // namespace
