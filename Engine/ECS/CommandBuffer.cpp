#include "ECS/CommandBuffer.hpp"

namespace Shape {

void CommandBuffer::PushCommand(std::function<void()> command) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_commands.push_back(std::move(command));
}

void CommandBuffer::Flush() {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& cmd : m_commands) {
        cmd();
    }
    m_commands.clear();
}

} // namespace Shape
