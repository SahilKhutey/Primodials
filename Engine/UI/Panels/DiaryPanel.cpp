#include "DiaryPanel.h"
#include "Core/Logger.hpp"

namespace ShapeEngine {

    DiaryPanel::DiaryPanel(Diary::EcosystemDiary* diary) 
        : m_diary(diary) {}

    void DiaryPanel::render() {
        if (!m_visible || !m_diary) return;
    }

} // namespace ShapeEngine
