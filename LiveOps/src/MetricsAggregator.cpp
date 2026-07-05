// MetricsAggregator.cpp
#include "MetricsAggregator.h"
namespace PolygonalPrimordials::LiveOps {
    double MetricSeries::min() const { return 0; }
    double MetricSeries::max() const { return 0; }
    double MetricSeries::mean() const { return 0; }
    double MetricSeries::p50() const { return 0; }
    double MetricSeries::p95() const { return 0; }
    double MetricSeries::p99() const { return 0; }
    double MetricSeries::stddev() const { return 0; }
    void MetricsAggregator::record(const std::string&, MetricValue, const std::unordered_map<std::string, std::string>&) {}
    void MetricsAggregator::record_batch(const std::vector<MetricSample>&) {}
    MetricSeries MetricsAggregator::get_series(const std::string&) const { return {}; }
    std::vector<std::string> MetricsAggregator::all_names() const { return {}; }
    std::string MetricsAggregator::export_json() const { return "{}"; }
    std::string MetricsAggregator::export_prometheus() const { return ""; }
    std::string MetricsAggregator::export_csv() const { return ""; }
    double MetricsAggregator::sum(const std::string&, std::chrono::seconds) const { return 0; }
    double MetricsAggregator::rate(const std::string&, std::chrono::seconds) const { return 0; }
    void MetricsAggregator::clear() {}
    double MetricsAggregator::compute(const MetricSeries&, const std::function<double(const std::vector<double>&)>&) const { return 0; }
}
