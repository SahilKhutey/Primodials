#pragma once

#include "Core/Platform.hpp"
#include "ShapeEngine/Math/Vector2.hpp"
#include <vector>
#include <string>
#include <optional>

namespace Shape::Renderer::Vulkan {

struct QueueFamilyIndices {
    std::optional<u32> graphics;
    std::optional<u32> compute;
    std::optional<u32> present;

    bool IsComplete() const {
        return graphics.has_value() && compute.has_value() && present.has_value();
    }
};

struct DeviceInfo {
    std::string name = "Generic Vulkan Device";
    u32 vendorId = 0;
    u32 deviceId = 0;
    u32 apiVersion = 0;
    usize dedicatedMemoryMB = 4096;
    bool isDiscreteGpu = true;
};

class VulkanContext {
public:
    VulkanContext() = default;
    ~VulkanContext() = default;

    bool Initialize(bool enableValidation = true) {
        m_validationEnabled = enableValidation;
        m_deviceInfo.name = "ShapeEngine Vulkan Hardware Acceleration";
        m_deviceInfo.dedicatedMemoryMB = 8192;
        m_deviceInfo.isDiscreteGpu = true;
        m_queueIndices.graphics = 0;
        m_queueIndices.compute = 0;
        m_queueIndices.present = 0;
        m_initialized = true;
        return true;
    }

    bool IsInitialized() const { return m_initialized; }
    bool IsValidationEnabled() const { return m_validationEnabled; }
    const DeviceInfo& GetDeviceInfo() const { return m_deviceInfo; }
    const QueueFamilyIndices& GetQueueIndices() const { return m_queueIndices; }

private:
    bool m_initialized = false;
    bool m_validationEnabled = true;
    DeviceInfo m_deviceInfo;
    QueueFamilyIndices m_queueIndices;
};

} // namespace Shape::Renderer::Vulkan
