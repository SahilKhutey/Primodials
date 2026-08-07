#include "Memory/ArenaAllocator.hpp"
#include <cstdlib>

namespace Shape {

ArenaAllocator::ArenaAllocator(usize sizeBytes, void* backingBuffer)
    : m_Buffer(static_cast<u8*>(backingBuffer))
    , m_Size(sizeBytes)
    , m_Offset(0)
    , m_OwnsBuffer(false) {
    if (m_Buffer == nullptr) {
        m_Buffer = static_cast<u8*>(std::malloc(sizeBytes));
        m_OwnsBuffer = true;
        SHAPE_ASSERT_MSG(m_Buffer != nullptr, "Failed to allocate backing memory for Arena (size: {})", sizeBytes);
    }
}

ArenaAllocator::~ArenaAllocator() {
    if (m_OwnsBuffer && m_Buffer) {
        std::free(m_Buffer);
    }
}

void* ArenaAllocator::Allocate(usize size, usize alignment) {
    uintptr_t currentAddress = reinterpret_cast<uintptr_t>(m_Buffer) + m_Offset;
    uintptr_t alignedAddress = AlignAddress(currentAddress, alignment);
    usize nextOffset = (alignedAddress - reinterpret_cast<uintptr_t>(m_Buffer)) + size;

    if (nextOffset > m_Size) {
        SHAPE_LOG_ERROR("Arena Allocator OOM! Capacity: {} bytes, Request: {} bytes", m_Size, size);
        return nullptr;
    }

    m_Offset = nextOffset;
    return reinterpret_cast<void*>(alignedAddress);
}

void ArenaAllocator::Deallocate(void* ptr) {
    // Arena allocators do not support freeing individual blocks of memory.
    (void)ptr;
}

void ArenaAllocator::Reset() {
    m_Offset = 0;
}

} // namespace Shape
