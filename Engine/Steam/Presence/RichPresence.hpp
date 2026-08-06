#pragma once

#include "Core/Platform.hpp"
#include <string>

namespace Shape::Steam {

class SteamContext;

class RichPresence {
public:
    explicit RichPresence(SteamContext* context);
    ~RichPresence() = default;

    void SetDisplay(const std::string& token, const std::string& param = "");
    void Clear();

    void SetInMenu();
    void SetWatching(u32 entityCount);

    const std::string& GetCurrentDisplay() const { return m_currentDisplay; }
    SteamContext* GetContext() const { return m_context; }

private:
    SteamContext* m_context = nullptr;
    std::string m_currentDisplay;
};

} // namespace Shape::Steam
