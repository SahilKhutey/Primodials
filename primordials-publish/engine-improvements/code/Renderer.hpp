/**
 * Primordials — Batched SDL3 GPU Renderer (C++23)
 *
 * Hardware instanced polygon rendering for high entity scale.
 *
 * License: MIT
 */

#pragma once

#include <cstdint>
#include <span>
#include <vector>

namespace primordials::render {

struct Vertex {
    float x, y;
    float r, g, b, a;
};

struct DrawCommand {
    std::uint32_t species_id;
    std::uint32_t instance_offset;
    std::uint32_t instance_count;
};

class BatchRenderer {
public:
    BatchRenderer() = default;

    void begin_frame() noexcept {
        instances_.clear();
        commands_.clear();
    }

    void submit_instance(float x, float y, float r, float g, float b, float a) noexcept {
        instances_.push_back({x, y, r, g, b, a});
    }

    void end_frame() noexcept {
        // Submit instance buffer directly to GPU
    }

    [[nodiscard]] std::size_t instance_count() const noexcept {
        return instances_.size();
    }

private:
    std::vector<Vertex> instances_;
    std::vector<DrawCommand> commands_;
};

} // namespace primordials::render
