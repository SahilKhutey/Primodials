// Shape/UI/LineChart.cpp
#include "Shape/UI/LineChart.h"
#include "Shape/UI/IOverlayUI.h"
#include "Shape/UI/Style.h"

#include <algorithm>

namespace Shape::UI {

    void LineChart::add_series(const std::string& name, const Math::Color& color) {
        LineChartSeries s;
        s.name = name;
        s.color = color;
        m_series.push_back(std::move(s));
    }

    void LineChart::push_value(size_t series_idx, float value) {
        if (series_idx >= m_series.size()) return;
        m_series[series_idx].values.push_back(value);
        if (m_series[series_idx].values.size() > m_max_history) {
            m_series[series_idx].values.erase(m_series[series_idx].values.begin());
        }
    }

    void LineChart::render(const Math::Vec2f& pos, const Math::Vec2f& size) {
        IOverlayUI* ui = Style::current_overlay();
        if (!ui) return;

        ui->begin_panel("line_chart", pos, size);

        // Compute scale
        if (m_auto_y) {
            m_ymin = 1e9f;
            m_ymax = -1e9f;
            for (const auto& s : m_series) {
                for (float v : s.values) {
                    m_ymin = std::min(m_ymin, v);
                    m_ymax = std::max(m_ymax, v);
                }
            }
            if (m_ymin == m_ymax) { m_ymin -= 1; m_ymax += 1; }
            const float pad = (m_ymax - m_ymin) * 0.1f;
            m_ymin -= pad;
            m_ymax += pad;
        }

        // Render legend
        for (const auto& s : m_series) {
            ui->label_colored(s.name.c_str(), s.color);
        }
        ui->separator();

        // Draw chart area
        for (const auto& s : m_series) {
            if (!s.visible) continue;
            std::vector<Math::Vec2f> points;
            const size_t n = s.values.size();
            points.reserve(n);
            for (size_t i = 0; i < n; ++i) {
                const float t = n > 1 ? static_cast<float>(i) / static_cast<float>(n - 1) : 0;
                const float y = (s.values[i] - m_ymin) / (m_ymax - m_ymin);
                points.push_back({t, 1.0f - y});
            }
            ui->line_chart(points, {size.x - 20, size.y - 40});
        }
        ui->end_panel();
    }

} // namespace Shape::UI
