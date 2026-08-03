#include "Memory/PoolAllocator.hpp"
#include <cstdlib>
#include <algorithm>

namespace Shape {

PoolAllocator::PoolAllocator(usize objectSize, usize objectAlignment, usize objectCount, void* backingBuffer)
    : m_Buffer(nullptr)
    , m_ObjectSize(std::max(objectSize, sizeof(Node)))
    , m_ObjectAlignment(std::max(objectAlignment, alignof(Node)))
    , m_ObjectCount(objectCount)
    , m_Size(0)
    , m_FreeCount(objectCount)
    , m_FreeList(nullptr)
    , m_OwnsBuffer(false) {
    
    usize stride = AlignAddress(m_ObjectSize, m_ObjectAlignment);
    m_Size = stride * objectCount;

    if (backingBuffer == nullptr) {
        // Allocate with extra padding to guarantee alignment of the first block
        m_Buffer = static_cast<u8*>(std::malloc(m_Size + m_ObjectAlignment));
        m_OwnsBuffer = true;
        SHAPE_ASSERT_MSG(m_Buffer != nullptr, "Failed to allocate memory for PoolAllocator (capacity: {})", objectCount);
    } else {
        m_Buffer = static_cast<u8*>(backingBuffer);
    }

    Reset();
}

PoolAllocator::~PoolAllocator() {
    if (m_OwnsBuffer && m_Buffer) {
        std::free(m_Buffer);
    }
}

void* PoolAllocator::Allocate(usize size, usize alignment) {
    SHAPE_ASSERT_MSG(size <= m_ObjectSize, "Request size {} exceeds Pool Allocator limit of {}", size, m_ObjectSize);
    SHAPE_ASSERT_MSG(alignment <= m_ObjectAlignment, "Request alignment {} exceeds Pool Allocator limit of {}", alignment, m_ObjectAlignment);

    if (m_FreeList == nullptr) {
        SHAPE_LOG_ERROR("Pool Allocator OOM! Capacity: {} objects", m_ObjectCount);
        return nullptr;
    }

    Node* block = m_FreeList;
    m_FreeList = m_FreeList->next;
    m_FreeCount--;

    return static_cast<void*>(block);
}

void PoolAllocator::Deallocate(void* ptr) {
    if (ptr == nullptr) return;

    // Safety checks (only compile in debug mode)
    #if defined(SHAPE_DEBUG)
    usize stride = AlignAddress(m_ObjectSize, m_ObjectAlignment);
    uintptr_t baseAddress = reinterpret_cast<uintptr_t>(m_Buffer);
    uintptr_t firstAligned = AlignAddress(baseAddress, m_ObjectAlignment);
    uintptr_t lastAligned = firstAligned + (m_ObjectCount - 1) * stride;
    uintptr_t ptrAddr = reinterpret_cast<uintptr_t>(ptr);

    SHAPE_ASSERT_MSG(ptrAddr >= firstAligned && ptrAddr <= lastAligned, "Pointer {} is out of bounds for PoolAllocator", ptr);
    SHAPE_ASSERT_MSG((ptrAddr - firstAligned) % stride == 0, "Pointer {} is misaligned to this PoolAllocator's blocks", ptr);
    #endif

    Node* block = static_cast<Node*>(ptr);
    block->next = m_FreeList;
    m_FreeList = block;
    m_FreeCount++;
}

void PoolAllocator::Reset() {
    m_FreeCount = m_ObjectCount;
    if (m_ObjectCount == 0) {
        m_FreeList = nullptr;
        return;
    }

    uintptr_t baseAddress = reinterpret_cast<uintptr_t>(m_Buffer);
    uintptr_t firstAligned = AlignAddress(baseAddress, m_ObjectAlignment);
    usize stride = AlignAddress(m_ObjectSize, m_ObjectAlignment);

    m_FreeList = reinterpret_cast<Node*>(firstAligned);
    Node* current = m_FreeList;

    for (usize i = 1; i < m_ObjectCount; ++i) {
        uintptr_t nextAddress = firstAligned + (i * stride);
        current->next = reinterpret_cast<Node*>(nextAddress);
        current = current->next;
    }
    current->next = nullptr;
}

} // namespace Shape
