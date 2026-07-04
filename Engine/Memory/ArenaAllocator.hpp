#pragma once

#include "Memory/Allocator.hpp"

namespace Shape {

class ArenaAllocator : public Allocator {
public:
    ArenaAllocator(usize sizeBytes, void* backingBuffer = nullptr);
    ~ArenaAllocator() override;

    void* Allocate(usize size, usize alignment = alignof(std::max_align_t)) override;
    void Deallocate(void* ptr) override;
    void Reset() override;

    // Getter diagnostics
    usize GetCapacity() const { return m_Size; }
    usize GetUsedMemory() const { return m_Offset; }

private:
    u8* m_Buffer;
    usize m_Size;
    usize m_Offset;
    bool m_OwnsBuffer;
};

} // namespace Shape
