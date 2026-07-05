// Shape/Rendering/Vulkan/VulkanRenderer.h
// Full Vulkan implementation of IRenderer
#pragma once

#include "Shape/Rendering/IRenderer.hpp"
#include "Shape/Rendering/Vulkan/VulkanContext.h"
#include "Shape/Rendering/Vulkan/VulkanSwapchain.h"
#include "Shape/Rendering/Vulkan/VulkanBuffer.h"
#include "Shape/Rendering/Vulkan/VulkanPipeline.h"
#include "Shape/Rendering/Vulkan/VulkanShader.h"
#include "Shape/Rendering/Vulkan/VulkanTexture.h"
#include "Shape/Rendering/Vulkan/VulkanCommandBuffer.h"

#include <memory>
#include <unordered_map>
#include <string>

namespace Shape::Rendering::Vulkan {

    class VulkanRenderer : public IRenderer {
    public:
        VulkanRenderer();
        ~VulkanRenderer() override;

        bool initialize(void* native_window, void* /*native_context*/, const RendererConfig& cfg) override;
        void shutdown() override;
        void begin_frame(const Math::Color& clear_color) override;
        void end_frame() override;
        void resize(Math::Vec2i size) override;
        Math::Vec2i drawable_size() const override;

        BufferHandle create_buffer(const void* data, const BufferDesc& desc) override;
        void update_buffer(BufferHandle h, const void* data, size_t size, size_t offset) override;
        void destroy_buffer(BufferHandle h) override;

        ShaderHandle create_shader(const std::vector<ShaderSource>& sources) override;
        void destroy_shader(ShaderHandle h) override;

        PipelineHandle create_pipeline(ShaderHandle shader, const VertexLayout& layout, PrimitiveType prim) override;
        void destroy_pipeline(PipelineHandle h) override;

        TextureHandle create_texture(const TextureDesc& desc, const void* data) override;
        void update_texture(TextureHandle h, const void* data, uint32_t mip) override;
        void destroy_texture(TextureHandle h) override;

        void set_viewport(int x, int y, int w, int h) override;
        void set_scissor(int x, int y, int w, int h) override;
        void set_clear_color(const Math::Color& c) override;
        void set_pipeline(PipelineHandle p) override;
        void bind_buffer(BufferHandle h, uint32_t binding) override;
        void bind_texture(TextureHandle h, uint32_t binding) override;
        void set_uniform_mat4(const char* name, const Math::Mat4f& m) override;
        void set_uniform_vec4(const char* name, const Math::Vec4f& v) override;
        void set_uniform_float(const char* name, float v) override;
        void set_uniform_int(const char* name, int v) override;
        void draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex) override;
        void draw_indexed(uint32_t index_count, uint32_t instance_count) override;

        const char* backend_name() const override { return "Vulkan 1.3"; }

    private:
        std::unique_ptr<VulkanContext> m_context;
        std::unique_ptr<VulkanSwapchain> m_swapchain;
        std::unique_ptr<VulkanCommandPool> m_cmd_pool;

        // Resources
        std::unordered_map<BufferHandle, std::unique_ptr<VulkanBuffer>> m_buffers;
        std::unordered_map<ShaderHandle, std::unique_ptr<VulkanShader>> m_shaders;
        std::unordered_map<PipelineHandle, std::unique_ptr<VulkanPipeline>> m_pipelines;
        std::unordered_map<TextureHandle, std::unique_ptr<VulkanTexture>> m_textures;

        BufferHandle m_next_buffer_id = 1;
        ShaderHandle m_next_shader_id = 1;
        PipelineHandle m_next_pipeline_id = 1;
        TextureHandle m_next_texture_id = 1;

        PipelineHandle m_current_pipeline = INVALID_PIPELINE;
        VkCommandBuffer m_current_cmd = VK_NULL_HANDLE;
        uint32_t m_current_frame = 0;
        Math::Color m_clear_color{0, 0, 0, 1};
        Math::Vec2i m_size{0, 0};

        // Descriptor management
        struct UniformBuffer {
            VkBuffer buffer = VK_NULL_HANDLE;
            VkDeviceMemory memory = VK_NULL_HANDLE;
            void* mapped = nullptr;
            size_t size = 0;
        };
        UniformBuffer m_uniform_buffer;
        std::unordered_map<std::string, size_t> m_uniform_offsets;
    };

} // namespace Shape::Rendering::Vulkan
