// Shape/Rendering/Vulkan/VulkanContext.cpp
#include "Shape/Rendering/Vulkan/VulkanContext.h"
#include "Shape/Core/Logger.hpp"
#include "Shape/Core/Assert.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <algorithm>
#include <cstring>
#include <vector>

namespace Shape::Rendering::Vulkan {

    VulkanContext::VulkanContext() = default;
    VulkanContext::~VulkanContext() { shutdown(); }

    namespace {
        VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
            VkDebugUtilsMessageSeverityFlagBitsEXT severity,
            VkDebugUtilsMessageTypeFlagsEXT /*type*/,
            const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
            void* /*p_user_data*/) {
            const char* sev = "INFO";
            if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) sev = "WARN";
            if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)   sev = "ERROR";
            SHAPE_LOG_WARN("Vulkan", "[{}] {}", sev, p_callback_data->pMessage);
            return VK_FALSE;
        }
    }

    bool VulkanContext::initialize(void* window_handle, const ContextConfig& cfg) {
        m_cfg = cfg;
        if (!create_surface(window_handle)) return false;
        if (!select_device()) return false;
        if (!create_logical_device()) return false;
        return true;
    }

    void VulkanContext::shutdown() {
        if (m_info.device) {
            vkDeviceWaitIdle(m_info.device);
            vkDestroyDevice(m_info.device, nullptr);
            m_info.device = VK_NULL_HANDLE;
        }
        if (m_info.surface) {
            destroy_surface();
        }
        if (m_info.instance) {
            vkDestroyInstance(m_info.instance, nullptr);
            m_info.instance = VK_NULL_HANDLE;
        }
    }

    bool VulkanContext::create_surface(void* window_handle) {
        SDL_Window* window = static_cast<SDL_Window*>(window_handle);

        // Create instance first
        VkApplicationInfo app_info{};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = "Shape Engine";
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "ShapeEngine";
        app_info.engineVersion = VK_MAKE_VERSION(0, 5, 0);
        app_info.apiVersion = m_cfg.vulkan_api_version;

        std::vector<const char*> extensions;
        if (window) {
            uint32_t sdl_ext_count = 0;
            const char* const* sdl_ext = SDL_Vulkan_GetInstanceExtensions(&sdl_ext_count);
            for (uint32_t i = 0; i < sdl_ext_count; ++i) extensions.push_back(sdl_ext[i]);
        }
        for (const auto& e : m_cfg.required_extensions) extensions.push_back(e.c_str());

        std::vector<const char*> layers;
        for (const auto& l : m_cfg.required_layers) layers.push_back(l.c_str());
        if (m_cfg.enable_validation) {
            layers.push_back("VK_LAYER_KHRONOS_validation");
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;
        create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        create_info.ppEnabledExtensionNames = extensions.data();
        create_info.enabledLayerCount = static_cast<uint32_t>(layers.size());
        create_info.ppEnabledLayerNames = layers.data();

        if (vkCreateInstance(&create_info, nullptr, &m_info.instance) != VK_SUCCESS) {
            SHAPE_LOG_ERROR("Vulkan", "Failed to create instance");
            return false;
        }

        // Debug callback
        if (m_cfg.enable_validation) {
            auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr(m_info.instance, "vkCreateDebugUtilsMessengerEXT"));
            if (func) {
                VkDebugUtilsMessengerCreateInfoEXT debug_info{};
                debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
                debug_info.messageSeverity =
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                debug_info.messageType =
                    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
                debug_info.pfnUserCallback = debug_callback;
                func(m_info.instance, &debug_info, nullptr, nullptr);
                m_validation_callback_set = true;
            }
        }

        // Create surface
        if (window) {
            if (!SDL_Vulkan_CreateSurface(window, m_info.instance, nullptr, &m_info.surface)) {
                SHAPE_LOG_ERROR("Vulkan", "Failed to create surface: {}", SDL_GetError());
                return false;
            }
        }
        return true;
    }

    void VulkanContext::destroy_surface() {
        if (m_info.surface) {
            vkDestroySurfaceKHR(m_info.instance, m_info.surface, nullptr);
            m_info.surface = VK_NULL_HANDLE;
        }
    }

    bool VulkanContext::select_device() {
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(m_info.instance, &device_count, nullptr);
        if (device_count == 0) {
            SHAPE_LOG_ERROR("Vulkan", "No Vulkan devices");
            return false;
        }
        std::vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(m_info.instance, &device_count, devices.data());

        // Pick best device
        VkPhysicalDevice best = VK_NULL_HANDLE;
        int best_score = -1;
        for (auto dev : devices) {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(dev, &props);
            int score = 0;
            if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score += 1000;
            score += static_cast<int>(props.limits.maxImageDimension2D);
            if (static_cast<int>(props.apiVersion) >= static_cast<int>(VK_API_VERSION_1_3)) score += 100;
            if (score > best_score) { best = dev; best_score = score; }
            SHAPE_LOG_INFO("Vulkan", "Device: {} (score {})", props.deviceName, score);
        }
        m_info.physical_device = best;
        vkGetPhysicalDeviceProperties(m_info.physical_device, &m_info.properties);
        vkGetPhysicalDeviceFeatures(m_info.physical_device, &m_info.features);
        vkGetPhysicalDeviceMemoryProperties(m_info.physical_device, &m_info.memory_properties);

        // Check feature support
        m_info.has_timeline_semaphores = (m_info.properties.apiVersion >= VK_API_VERSION_1_2);
        m_info.has_buffer_device_address = m_info.features.bufferDeviceAddress;
        m_info.has_dynamic_rendering = m_info.properties.apiVersion >= VK_API_VERSION_1_3;
        m_info.has_synchronization2 = m_info.properties.apiVersion >= VK_API_VERSION_1_3;

        return best != VK_NULL_HANDLE;
    }

    bool VulkanContext::create_logical_device() {
        // Find queue families
        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_info.physical_device, &queue_family_count, nullptr);
        std::vector<VkQueueFamilyProperties> families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(m_info.physical_device, &queue_family_count, families.data());

        for (uint32_t i = 0; i < families.size(); ++i) {
            if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_info.graphics_family = i;
            }
            if (families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
                m_info.compute_family = i;
            }
            if (families[i].queueFlags & VK_QUEUE_TRANSFER_BIT && !(families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                m_info.transfer_family = i;
            }
            if (m_info.surface) {
                VkBool32 present_support = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(m_info.physical_device, i, m_info.surface, &present_support);
                if (present_support) m_info.present_family = i;
            }
        }

        // Default fallbacks
        if (m_info.compute_family == UINT32_MAX) m_info.compute_family = m_info.graphics_family;
        if (m_info.transfer_family == UINT32_MAX) m_info.transfer_family = m_info.graphics_family;
        if (m_info.present_family == UINT32_MAX) m_info.present_family = m_info.graphics_family;

        // Create queues
        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        std::vector<uint32_t> unique_families = {
            m_info.graphics_family,
            m_info.compute_family,
            m_info.present_family
        };
        unique_families.erase(std::unique(unique_families.begin(), unique_families.end()), unique_families.end());

        const float queue_priority = 1.0f;
        for (auto fam : unique_families) {
            VkDeviceQueueCreateInfo qci{};
            qci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            qci.queueFamilyIndex = fam;
            qci.queueCount = 1;
            qci.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(qci);
        }

        // Required extensions
        std::vector<const char*> device_extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        if (m_info.has_buffer_device_address) {
            device_extensions.push_back("VK_KHR_buffer_device_address");
        }

        VkPhysicalDeviceFeatures features{};
        features.samplerAnisotropy = VK_TRUE;
        features.fillModeNonSolid = VK_TRUE;
        features.wideLines = VK_TRUE;
        if (m_info.has_buffer_device_address) {
            features.bufferDeviceAddress = VK_TRUE;
        }

        VkDeviceCreateInfo device_info{};
        device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
        device_info.pQueueCreateInfos = queue_create_infos.data();
        device_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
        device_info.ppEnabledExtensionNames = device_extensions.data();
        device_info.pEnabledFeatures = &features;

        if (vkCreateDevice(m_info.physical_device, &device_info, nullptr, &m_info.device) != VK_SUCCESS) {
            SHAPE_LOG_ERROR("Vulkan", "Failed to create device");
            return false;
        }

        vkGetDeviceQueue(m_info.device, m_info.graphics_family, 0, &m_info.graphics_queue);
        if (m_info.compute_family != m_info.graphics_family) {
            vkGetDeviceQueue(m_info.device, m_info.compute_family, 0, &m_info.compute_queue);
        } else {
            m_info.compute_queue = m_info.graphics_queue;
        }
        if (m_info.transfer_family != m_info.graphics_family) {
            vkGetDeviceQueue(m_info.device, m_info.transfer_family, 0, &m_info.transfer_queue);
        } else {
            m_info.transfer_queue = m_info.graphics_queue;
        }
        return true;
    }

    uint32_t VulkanContext::find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) const {
        for (uint32_t i = 0; i < m_info.memory_properties.memoryTypeCount; ++i) {
            if ((type_filter & (1 << i)) &&
                (m_info.memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        SHAPE_ASSERT_MSG(false, "Vulkan: failed to find suitable memory type");
        return 0;
    }

    VkFormat VulkanContext::find_supported_format(const std::vector<VkFormat>& candidates,
                                                    VkImageTiling tiling,
                                                    VkFormatFeatureFlags features) const {
        for (auto fmt : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_info.physical_device, fmt, &props);
            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) return fmt;
            if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) return fmt;
        }
        SHAPE_ASSERT_MSG(false, "Vulkan: failed to find supported format");
        return VK_FORMAT_UNDEFINED;
    }

    VkShaderModule VulkanContext::create_shader_module(const std::vector<uint8_t>& spirv) const {
        VkShaderModuleCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        info.codeSize = spirv.size();
        info.pCode = reinterpret_cast<const uint32_t*>(spirv.data());
        VkShaderModule mod;
        if (vkCreateShaderModule(m_info.device, &info, nullptr, &mod) != VK_SUCCESS) {
            return VK_NULL_HANDLE;
        }
        return mod;
    }

    void VulkanContext::set_object_name(VkObjectType type, uint64_t handle, const char* name) const {
        if (!m_info.device) return;
        auto func = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(
            vkGetDeviceProcAddr(m_info.device, "vkSetDebugUtilsObjectNameEXT"));
        if (!func) return;
        VkDebugUtilsObjectNameInfoEXT info{};
        info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        info.objectType = type;
        info.objectHandle = handle;
        info.pObjectName = name;
        func(m_info.device, &info);
    }

} // namespace Shape::Rendering::Vulkan
