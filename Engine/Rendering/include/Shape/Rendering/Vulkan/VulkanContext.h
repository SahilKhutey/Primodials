// Shape/Rendering/Vulkan/VulkanContext.h
// Top-level Vulkan context
#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>
#include <vector>
#include <string>

namespace Shape::Rendering::Vulkan {

    struct ContextConfig {
        bool enable_validation = false;
        bool enable_debug_utils = false;
        uint32_t vulkan_api_version = VK_API_VERSION_1_3;
        std::vector<std::string> required_extensions;
        std::vector<std::string> required_layers;
        bool prefer_discrete_gpu = true;
    };

    struct ContextInfo {
        VkInstance instance = VK_NULL_HANDLE;
        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VkQueue graphics_queue = VK_NULL_HANDLE;
        VkQueue compute_queue = VK_NULL_HANDLE;
        VkQueue transfer_queue = VK_NULL_HANDLE;
        VkSurfaceKHR surface = VK_NULL_HANDLE;

        uint32_t graphics_family = UINT32_MAX;
        uint32_t compute_family = UINT32_MAX;
        uint32_t transfer_family = UINT32_MAX;
        uint32_t present_family = UINT32_MAX;

        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures features{};
        VkPhysicalDeviceMemoryProperties memory_properties{};

        bool has_timeline_semaphores = false;
        bool has_buffer_device_address = false;
        bool has_dynamic_rendering = false;
        bool has_synchronization2 = false;
    };

    class VulkanContext {
    public:
        VulkanContext();
        ~VulkanContext();

        bool initialize(void* window_handle, const ContextConfig& cfg = {});
        void shutdown();

        bool is_valid() const { return m_info.instance != VK_NULL_HANDLE; }

        // Surface creation (after window ready)
        bool create_surface(void* window_handle);
        void destroy_surface();

        // Device selection
        bool select_device();
        bool create_logical_device();

        // Access
        const ContextInfo& info() const { return m_info; }
        VkInstance instance() const { return m_info.instance; }
        VkPhysicalDevice physical() const { return m_info.physical_device; }
        VkDevice device() const { return m_info.device; }
        VkSurfaceKHR surface() const { return m_info.surface; }

        // Queues
        VkQueue graphics_queue() const { return m_info.graphics_queue; }
        VkQueue compute_queue() const { return m_info.compute_queue; }
        VkQueue transfer_queue() const { return m_info.transfer_queue; }
        uint32_t graphics_family() const { return m_info.graphics_family; }
        uint32_t compute_family() const { return m_info.compute_family; }
        uint32_t transfer_family() const { return m_info.transfer_family; }
        uint32_t present_family() const { return m_info.present_family; }

        // Helpers
        uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) const;
        VkFormat find_supported_format(const std::vector<VkFormat>& candidates,
                                       VkImageTiling tiling,
                                       VkFormatFeatureFlags features) const;
        VkShaderModule create_shader_module(const std::vector<uint8_t>& spirv) const;

        // Validation
        void set_object_name(VkObjectType type, uint64_t handle, const char* name) const;

    private:
        ContextInfo m_info;
        ContextConfig m_cfg;
        bool m_validation_callback_set = false;
    };

} // namespace Shape::Rendering::Vulkan
