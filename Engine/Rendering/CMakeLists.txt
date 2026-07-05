# Engine/Rendering/CMakeLists.txt
add_library(ShapeRenderingVulkan STATIC
    "include/Shape/Rendering/Vulkan/VulkanContext.h"
    "include/Shape/Rendering/Vulkan/VulkanInstance.h"
    "include/Shape/Rendering/Vulkan/VulkanDevice.h"
    "include/Shape/Rendering/Vulkan/VulkanSwapchain.h"
    "include/Shape/Rendering/Vulkan/VulkanCommandBuffer.h"
    "include/Shape/Rendering/Vulkan/VulkanPipeline.h"
    "include/Shape/Rendering/Vulkan/VulkanBuffer.h"
    "include/Shape/Rendering/Vulkan/VulkanTexture.h"
    "include/Shape/Rendering/Vulkan/VulkanShader.h"
    "include/Shape/Rendering/Vulkan/VulkanRenderer.h"
    "include/Shape/Rendering/Vulkan/VulkanRenderPass.h"
    "include/Shape/Rendering/Vulkan/VulkanDescriptor.h"
    "include/Shape/Rendering/Vulkan/VulkanUtils.h"
    "src/Vulkan/VulkanContext.cpp"
    "src/Vulkan/VulkanInstance.cpp"
    "src/Vulkan/VulkanDevice.cpp"
    "src/Vulkan/VulkanSwapchain.cpp"
    "src/Vulkan/VulkanCommandBuffer.cpp"
    "src/Vulkan/VulkanPipeline.cpp"
    "src/Vulkan/VulkanBuffer.cpp"
    "src/Vulkan/VulkanTexture.cpp"
    "src/Vulkan/VulkanShader.cpp"
    "src/Vulkan/VulkanRenderer.cpp"
    "src/Vulkan/VulkanRenderPass.cpp"
    "src/Vulkan/VulkanDescriptor.cpp"
    "src/Vulkan/VulkanUtils.cpp"
)

target_include_directories(ShapeRenderingVulkan PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Engine>
)

target_link_libraries(ShapeRenderingVulkan
    PUBLIC ShapeEngine
    PRIVATE SDL3::SDL3-static
)

find_package(Vulkan)
if(Vulkan_FOUND)
    target_link_libraries(ShapeRenderingVulkan PRIVATE Vulkan::Vulkan)
    target_compile_definitions(ShapeRenderingVulkan PRIVATE SHAPE_VULKAN_ENABLED)
endif()
