#include "SystemProfiler.hpp"
#include "imgui.h"

namespace ShapeEditor {

void SystemProfiler::Render() {
    ImGui::Begin("System Profiler");
    
    if (m_CurrentFrameRecords.empty()) {
        ImGui::Text("No profile records for current frame.");
    } else {
        for (const auto& record : m_CurrentFrameRecords) {
            ImGui::Text("%s: %.3f ms", record.name.c_str(), record.timeMs);
        }
    }

    ImGui::Separator();
    
    // Calculate average frame time
    float avgFrameTime = 0.0f;
    for (int i = 0; i < MAX_HISTORY; ++i) {
        avgFrameTime += m_FrameTimes[i];
    }
    avgFrameTime /= MAX_HISTORY;

    ImGui::Text("Average Frame Time: %.3f ms (%.1f FPS)", avgFrameTime, avgFrameTime > 0.0f ? 1000.0f / avgFrameTime : 0.0f);
    ImGui::PlotLines("Frame Times (ms)", m_FrameTimes, MAX_HISTORY, m_HistoryOffset, nullptr, 0.0f, 33.3f, ImVec2(0, 80));

    ImGui::End();
}

void SystemProfiler::PushRecord(const std::string& name, f32 timeMs) {
    m_CurrentFrameRecords.push_back({name, timeMs});
}

void SystemProfiler::EndFrame() {
    float totalFrameTime = 0.0f;
    for (const auto& record : m_CurrentFrameRecords) {
        totalFrameTime += record.timeMs;
    }
    
    m_FrameTimes[m_HistoryOffset] = totalFrameTime;
    m_HistoryOffset = (m_HistoryOffset + 1) % MAX_HISTORY;
    
    m_CurrentFrameRecords.clear();
}

} // namespace ShapeEditor
