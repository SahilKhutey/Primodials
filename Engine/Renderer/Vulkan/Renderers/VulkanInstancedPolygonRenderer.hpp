#pragma once

#include "Renderer/Vulkan/Core/VulkanContext.hpp"
#include "Renderer/Vulkan/Core/VulkanSwapchain.hpp"
#include "Renderer/Vulkan/Resources/VulkanBuffer.hpp"
#include "ShapeEngine/Math/Vector2.hpp"

namespace Shape::Renderer::Vulkan {

struct VulkanGPUInstanceData {
    Math::Vector2f position;
    Math::Vector2f scale;
    f32 rotation = 0.0f;
    u32 speciesId = 0;
    u32 flags = 0;
    u32 generation = 0;
};

class VulkanInstancedPolygonRenderer {
public:
    VulkanInstancedPolygonRenderer() = default;
    ~VulkanInstancedPolygonRenderer() = default;

    bool Initialize(VulkanContext* context, VulkanSwapchain* swapchain) {
        if (!context || !swapchain || !context->IsInitialized()) return false;
        m_context = context;
        m_swapchain = swapchain;
        m_instanceBuffer.Initialize(context, sizeof(VulkanGPUInstanceData) * 10000, BufferType::Storage, true);
        m_initialized = true;
        return true;
    }

    void UploadInstances(const VulkanGPUInstanceData* data, u32 count) {
        if (!m_initialized) return;
        m_lastDrawCount = count;
        usize bytes = sizeof(VulkanGPUInstanceData) * count;
        if (bytes > m_instanceBuffer.GetSizeBytes()) {
            m_instanceBuffer.Initialize(m_context, bytes * 2, BufferType::Storage, true);
        }
        m_instanceBuffer.Upload(data, bytes, 0);
    }

    u32 GetLastDrawCount() const { return m_lastDrawCount; }
    bool IsInitialized() const { return m_initialized; }

private:
    VulkanContext* m_context = nullptr;
    VulkanSwapchain* m_swapchain = nullptr;
    VulkanBuffer m_instanceBuffer;
    u32 m_lastDrawCount = 0;
    bool m_initialized = false;
};

} // namespace Shape::Renderer::Vulkan
