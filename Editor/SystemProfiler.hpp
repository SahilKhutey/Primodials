#pragma once
#include "Core/Platform.hpp"
#include <string>
#include <vector>

namespace ShapeEditor {

class SystemProfiler {
public:
    void Render();

    void PushRecord(const std::string& name, f32 timeMs);
    void EndFrame();

private:
    struct Record {
        std::string name;
        f32 timeMs;
    };
    
    std::vector<Record> m_CurrentFrameRecords;
    
    // For histogram
    static const int MAX_HISTORY = 100;
    float m_FrameTimes[MAX_HISTORY] = {0};
    int m_HistoryOffset = 0;
};

} // namespace ShapeEditor
