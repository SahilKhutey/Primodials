#pragma once

#include "Memory/Allocator.hpp"

namespace Shape {

class PoolAllocator : public Allocator {
public:
    PoolAllocator(usize objectSize, usize objectAlignment, usize objectCount, void* backingBuffer = nullptr);
    ~PoolAllocator() override;

    void* Allocate(usize size, usize alignment = alignof(std::max_align_t)) override;
    void Deallocate(void* ptr) override;
    void Reset() override;

    usize GetObjectSize() const { return m_ObjectSize; }
    usize GetCapacity() const { return m_ObjectCount; }
    usize GetFreeCount() const { return m_FreeCount; }

private:
    struct Node {
        Node* next;
    };

    u8* m_Buffer;
    usize m_ObjectSize;
    usize m_ObjectAlignment;
    usize m_ObjectCount;
    usize m_Size;
    usize m_FreeCount;

    Node* m_FreeList;
    bool m_OwnsBuffer;
};

} // namespace Shape
