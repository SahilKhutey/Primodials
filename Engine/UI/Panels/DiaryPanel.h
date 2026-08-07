#pragma once

#include "Diary/EcosystemDiary.h"
#include <memory>

namespace ShapeEngine {

    class DiaryPanel {
    public:
        explicit DiaryPanel(Diary::EcosystemDiary* diary);
        ~DiaryPanel() = default;

        void render();
        void show() { m_visible = true; }
        void hide() { m_visible = false; }
        bool isVisible() const { return m_visible; }

    private:
        Diary::EcosystemDiary* m_diary;
        bool m_visible = false;
        [[maybe_unused]] bool m_showSummary = false;
        [[maybe_unused]] char m_searchFilter[64] = "";
    };

} // namespace ShapeEngine
