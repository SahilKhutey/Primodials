#pragma once

#include "Renderer/Vulkan/Core/VulkanContext.hpp"
#include <vector>
#include <cstring>

namespace Shape::Renderer::Vulkan {

enum class BufferType {
    Vertex,
    Index,
    Uniform,
    Storage,
    Staging
};

class VulkanBuffer {
public:
    VulkanBuffer() = default;
    ~VulkanBuffer() { Shutdown(); }

    bool Initialize(VulkanContext* context, usize sizeBytes, BufferType type, bool hostVisible = false) {
        if (!context || !context->IsInitialized()) return false;
        m_context = context;
        m_sizeBytes = sizeBytes;
        m_type = type;
        m_hostVisible = hostVisible;
        m_bufferData.resize(sizeBytes, 0);
        m_initialized = true;
        return true;
    }

    void Shutdown() {
        m_bufferData.clear();
        m_sizeBytes = 0;
        m_initialized = false;
    }

    void Upload(const void* data, usize sizeBytes, usize offset = 0) {
        if (!m_initialized || offset + sizeBytes > m_sizeBytes) return;
        std::memcpy(m_bufferData.data() + offset, data, sizeBytes);
    }

    usize GetSizeBytes() const { return m_sizeBytes; }
    BufferType GetType() const { return m_type; }
    bool IsHostVisible() const { return m_hostVisible; }
    bool IsInitialized() const { return m_initialized; }
    const u8* GetData() const { return m_bufferData.data(); }

private:
    VulkanContext* m_context = nullptr;
    usize m_sizeBytes = 0;
    BufferType m_type = BufferType::Vertex;
    bool m_hostVisible = false;
    bool m_initialized = false;
    std::vector<u8> m_bufferData;
};

} // namespace Shape::Renderer::Vulkan
