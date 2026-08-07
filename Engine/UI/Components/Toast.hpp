#pragma once

#include "Core/Platform.hpp"
#include <string>
#include <vector>
#include <functional>

namespace Shape::UI {

enum class ToastLevel { Info, Success, Warning, Error };

struct Toast {
    u64 id = 0;
    std::string title;
    std::string message;
    ToastLevel level = ToastLevel::Info;
    f32 duration = 4.0f;
    f32 timeRemaining = 4.0f;
};

class ToastManager {
public:
    static ToastManager& Get() {
        static ToastManager instance;
        return instance;
    }

    u64 Show(const std::string& title, const std::string& message, ToastLevel level = ToastLevel::Info, f32 duration = 4.0f);
    void ShowAchievementUnlock(const std::string& name, const std::string& description);
    void Dismiss(u64 id);
    void Clear();

    void Update(f32 dt);
    const std::vector<Toast>& GetActiveToasts() const { return m_toasts; }

private:
    ToastManager() = default;
    std::vector<Toast> m_toasts;
    u64 m_nextId = 1;
};

} // namespace Shape::UI
