// AnomalyDetector.cpp
#include "AnomalyDetector.h"
namespace PolygonalPrimordials::LiveOps {
    AnomalyDetector::AnomalyDetector(MetricsAggregator& agg, const AnomalyConfig& cfg) : m_agg(agg), m_cfg(cfg) {}
    std::vector<Anomaly> AnomalyDetector::detect() { return {}; }
    std::vector<Anomaly> AnomalyDetector::detect_zscore(const std::string&) { return {}; }
    std::vector<Anomaly> AnomalyDetector::detect_threshold(const std::string&) { return {}; }
    std::vector<Anomaly> AnomalyDetector::detect_stale(const std::string&) { return {}; }
    std::vector<Anomaly> AnomalyDetector::detect_rate_of_change(const std::string&) { return {}; }
}
