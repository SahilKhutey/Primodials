#include <catch2/catch_test_macros.hpp>
#include "Renderer/Vulkan/Core/VulkanContext.hpp"
#include "Renderer/Vulkan/Core/VulkanSwapchain.hpp"
#include "Renderer/Vulkan/Resources/VulkanBuffer.hpp"
#include "Renderer/Vulkan/Renderers/VulkanInstancedPolygonRenderer.hpp"
#include "Renderer/Vulkan/EmbeddedShaders.hpp"

TEST_CASE("Vulkan Renderer Backend Subsystem - Initialization, Buffers, and Instancing", "[Vulkan]") {
    SECTION("VulkanContext device scoring and queue indices") {
        Shape::Renderer::Vulkan::VulkanContext context;
        REQUIRE(context.Initialize(true));
        REQUIRE(context.IsInitialized());
        REQUIRE(context.IsValidationEnabled());
        
        const auto& info = context.GetDeviceInfo();
        REQUIRE(info.isDiscreteGpu);
        REQUIRE(info.dedicatedMemoryMB >= 4096);
        
        const auto& queues = context.GetQueueIndices();
        REQUIRE(queues.IsComplete());
    }

    SECTION("VulkanSwapchain extent and triple-buffering") {
        Shape::Renderer::Vulkan::VulkanContext context;
        context.Initialize(true);

        Shape::Renderer::Vulkan::VulkanSwapchain swapchain;
        REQUIRE(swapchain.Initialize(&context, 1920, 1080, true));
        REQUIRE(swapchain.GetWidth() == 1920);
        REQUIRE(swapchain.GetHeight() == 1080);
        REQUIRE(swapchain.GetImageCount() == 3);
        REQUIRE(swapchain.IsVSync());
    }

    SECTION("VulkanBuffer device-local and staging uploads") {
        Shape::Renderer::Vulkan::VulkanContext context;
        context.Initialize(true);

        Shape::Renderer::Vulkan::VulkanBuffer buffer;
        REQUIRE(buffer.Initialize(&context, 1024, Shape::Renderer::Vulkan::BufferType::Storage, true));
        REQUIRE(buffer.GetSizeBytes() == 1024);
        REQUIRE(buffer.IsHostVisible());

        u32 data[4] = { 10, 20, 30, 40 };
        buffer.Upload(data, sizeof(data), 0);
        
        const u32* readBack = reinterpret_cast<const u32*>(buffer.GetData());
        REQUIRE(readBack[0] == 10);
        REQUIRE(readBack[3] == 40);
    }

    SECTION("VulkanInstancedPolygonRenderer instance SSBO upload") {
        Shape::Renderer::Vulkan::VulkanContext context;
        context.Initialize(true);

        Shape::Renderer::Vulkan::VulkanSwapchain swapchain;
        swapchain.Initialize(&context, 1920, 1080, true);

        Shape::Renderer::Vulkan::VulkanInstancedPolygonRenderer renderer;
        REQUIRE(renderer.Initialize(&context, &swapchain));

        std::vector<Shape::Renderer::Vulkan::VulkanGPUInstanceData> instances(500);
        for (u32 i = 0; i < 500; ++i) {
            instances[i].position = { static_cast<f32>(i), static_cast<f32>(i * 2) };
            instances[i].scale = { 2.0f, 2.0f };
        }

        renderer.UploadInstances(instances.data(), 500);
        REQUIRE(renderer.GetLastDrawCount() == 500);
    }

    SECTION("EmbeddedShaders SPIR-V module lookup") {
        const auto& shaders = Shape::Renderer::Vulkan::EmbeddedShaders::GetAll();
        REQUIRE(shaders.empty()); // No custom SPV files compiled yet
    }
}
