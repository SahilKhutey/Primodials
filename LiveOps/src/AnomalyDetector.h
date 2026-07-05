// LiveOps/src/AnomalyDetector.h
// Statistical anomaly detection
#pragma once

#include "MetricsAggregator.h"
#include <string>
#include <vector>
#include <functional>
#include <chrono>

namespace PolygonalPrimordials::LiveOps {

    enum class AnomalySeverity : uint8_t {
        Info, Warning, Critical
    };

    struct Anomaly {
        std::string metric_name;
        AnomalySeverity severity = AnomalySeverity::Info;
        std::string description;
        double current_value = 0;
        double expected_value = 0;
        double deviation_sigma = 0;
        std::chrono::system_clock::time_point detected_at;
    };

    struct AnomalyConfig {
        // Z-score threshold
        double warning_sigma = 2.0;
        double critical_sigma = 4.0;

        // Absolute thresholds
        std::unordered_map<std::string, double> max_thresholds;
        std::unordered_map<std::string, double> min_thresholds;

        // Window
        std::chrono::seconds baseline_window = std::chrono::hours(24);
        std::chrono::seconds recent_window = std::chrono::minutes(15);
    };

    class AnomalyDetector {
    public:
        AnomalyDetector(MetricsAggregator& agg, const AnomalyConfig& cfg = {});

        void set_config(const AnomalyConfig& cfg) { m_cfg = cfg; }
        const AnomalyConfig& config() const { return m_cfg; }

        // Detect
        std::vector<Anomaly> detect();

        // Specific detectors
        std::vector<Anomaly> detect_zscore(const std::string& metric);
        std::vector<Anomaly> detect_threshold(const std::string& metric);
        std::vector<Anomaly> detect_stale(const std::string& metric);
        std::vector<Anomaly> detect_rate_of_change(const std::string& metric);

    private:
        MetricsAggregator& m_agg;
        AnomalyConfig m_cfg;
    };

} // namespace
