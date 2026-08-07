#include "UI/Components/Toast.hpp"
#include <algorithm>

namespace Shape::UI {

u64 ToastManager::Show(const std::string& title, const std::string& message, ToastLevel level, f32 duration) {
    Toast t;
    t.id = m_nextId++;
    t.title = title;
    t.message = message;
    t.level = level;
    t.duration = duration;
    t.timeRemaining = duration;
    m_toasts.push_back(t);
    return t.id;
}

void ToastManager::ShowAchievementUnlock(const std::string& name, const std::string& description) {
    Show("🏆 Achievement Unlocked!", name + "\n" + description, ToastLevel::Success, 6.0f);
}

void ToastManager::Dismiss(u64 id) {
    std::erase_if(m_toasts, [id](const Toast& t) { return t.id == id; });
}

void ToastManager::Clear() {
    m_toasts.clear();
}

void ToastManager::Update(f32 dt) {
    for (auto& t : m_toasts) {
        t.timeRemaining -= dt;
    }
    std::erase_if(m_toasts, [](const Toast& t) { return t.timeRemaining <= 0.0f; });
}

} // namespace Shape::UI
