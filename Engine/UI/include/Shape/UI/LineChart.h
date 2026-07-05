// Shape/UI/LineChart.h
// Multi-series line chart for time-series data
#pragma once

#include "Shape/Math/Vec2.hpp"
#include "Shape/Math/Color.hpp"
#include <vector>
#include <string>
#include <cstdint>

namespace Shape::UI {

    struct LineChartSeries {
        std::string name;
        Math::Color color{1, 1, 1, 1};
        std::vector<float> values;
        bool visible = true;
    };

    class LineChart {
    public:
        LineChart() = default;
        ~LineChart() = default;

        void add_series(const std::string& name, const Math::Color& color);
        void push_value(size_t series_idx, float value);
        void set_max_history(size_t n) { m_max_history = n; }
        void set_y_range(float ymin, float ymax) { m_ymin = ymin; m_ymax = ymax; m_auto_y = false; }
        void set_auto_y() { m_auto_y = true; }

        size_t series_count() const { return m_series.size(); }
        LineChartSeries& series(size_t i) { return m_series[i]; }
        const LineChartSeries& series(size_t i) const { return m_series[i]; }

        // Render to overlay
        void render(const Math::Vec2f& pos, const Math::Vec2f& size);

    private:
        std::vector<LineChartSeries> m_series;
        size_t m_max_history = 600;
        float m_ymin = 0, m_ymax = 1;
        bool m_auto_y = true;
    };

} // namespace Shape::UI
