#include "Steam/Presence/RichPresence.hpp"
#include "Steam/SteamContext.hpp"

namespace Shape::Steam {

RichPresence::RichPresence(SteamContext* context)
    : m_context(context) {}

void RichPresence::SetDisplay(const std::string& token, const std::string& param) {
    m_currentDisplay = param.empty() ? token : (token + "#" + param);
}

void RichPresence::Clear() {
    m_currentDisplay.clear();
}

void RichPresence::SetInMenu() {
    SetDisplay("in_menu");
}

void RichPresence::SetWatching(u32 entityCount) {
    SetDisplay("watching", std::to_string(entityCount));
}

} // namespace Shape::Steam
