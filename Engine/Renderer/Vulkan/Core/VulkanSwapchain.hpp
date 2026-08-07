#pragma once

#include "Renderer/Vulkan/Core/VulkanContext.hpp"

namespace Shape::Renderer::Vulkan {

class VulkanSwapchain {
public:
    VulkanSwapchain() = default;
    ~VulkanSwapchain() = default;

    bool Initialize(VulkanContext* context, u32 width, u32 height, bool vsync = true) {
        if (!context || !context->IsInitialized()) return false;
        m_context = context;
        m_width = width;
        m_height = height;
        m_vsync = vsync;
        m_imageCount = 3; // Triple buffering
        m_initialized = true;
        return true;
    }

    bool Recreate(u32 width, u32 height) {
        m_width = width;
        m_height = height;
        return true;
    }

    u32 GetWidth() const { return m_width; }
    u32 GetHeight() const { return m_height; }
    u32 GetImageCount() const { return m_imageCount; }
    bool IsVSync() const { return m_vsync; }
    bool IsInitialized() const { return m_initialized; }

private:
    VulkanContext* m_context = nullptr;
    u32 m_width = 1920;
    u32 m_height = 1080;
    u32 m_imageCount = 3;
    bool m_vsync = true;
    bool m_initialized = false;
};

} // namespace Shape::Renderer::Vulkan
