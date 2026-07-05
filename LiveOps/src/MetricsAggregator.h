// LiveOps/src/MetricsAggregator.h
// Collects and aggregates metrics from many sources
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <chrono>
#include <variant>
#include <cstdint>
#include <functional>

namespace PolygonalPrimordials::LiveOps {

    using MetricValue = std::variant<int64_t, double, bool, std::string>;

    struct MetricSample {
        std::string name;
        MetricValue value;
        std::chrono::system_clock::time_point timestamp;
        std::unordered_map<std::string, std::string> tags;
    };

    struct MetricSeries {
        std::string name;
        std::vector<MetricSample> samples;
        size_t max_samples = 1440;  // 24 hours at 1/min

        // Stats
        double min() const;
        double max() const;
        double mean() const;
        double p50() const;
        double p95() const;
        double p99() const;
        double stddev() const;
    };

    class MetricsAggregator {
    public:
        void record(const std::string& name, MetricValue value,
                    const std::unordered_map<std::string, std::string>& tags = {});

        // Bulk
        void record_batch(const std::vector<MetricSample>& samples);

        // Query
        MetricSeries get_series(const std::string& name) const;
        std::vector<std::string> all_names() const;

        // Export
        std::string export_json() const;
        std::string export_prometheus() const;
        std::string export_csv() const;

        // Aggregations
        double sum(const std::string& name, std::chrono::seconds window) const;
        double rate(const std::string& name, std::chrono::seconds window) const;

        // Reset
        void clear();

    private:
        mutable std::mutex m_mutex;
        std::unordered_map<std::string, MetricSeries> m_series;

        double compute(const MetricSeries& s, const std::function<double(const std::vector<double>&)>& op) const;
    };

} // namespace
