#pragma once

#include "Memory/Allocator.hpp"
#include "Core/Assert.hpp"
#include <cstring>

namespace Shape {

// ─────────────────────────────────────────────────────────────────────────────
//  FreeListAllocator — variable-size, first-fit strategy.
//
//  Characteristics:
//    - Each allocation is preceded by a header storing the block size and
//      whether it is free.
//    - Free blocks are coalesced on Deallocate.
//    - Suitable for level-load, asset streaming, and other non-hot-path use.
//    - NOT safe for the simulation hot loop (use FrameAllocator there).
// ─────────────────────────────────────────────────────────────────────────────
class FreeListAllocator final : public Allocator {
public:
    explicit FreeListAllocator(usize capacity)
        : m_Buffer(new u8[capacity])
        , m_Capacity(capacity) {
        // Initialise with one giant free block
        BlockHeader* first = header_at(0);
        first->size   = capacity - sizeof(BlockHeader);
        first->free   = true;
        first->next   = nullptr;
    }

    ~FreeListAllocator() override = default;

    void* Allocate(usize size, usize alignment = alignof(std::max_align_t)) override {
        // Round size up to satisfy alignment
        usize alignedSize = AlignAddress(size, alignment);
        alignedSize = alignedSize < sizeof(BlockHeader*) ? sizeof(BlockHeader*) : alignedSize;

        BlockHeader* prev    = nullptr;
        BlockHeader* current = header_at(0);

        while (current) {
            if (current->free && current->size >= alignedSize + sizeof(BlockHeader)) {
                split(current, alignedSize);
                current->free = false;
                return reinterpret_cast<void*>(
                    reinterpret_cast<u8*>(current) + sizeof(BlockHeader));
            }
            prev    = current;
            current = current->next;
        }

        SHAPE_ASSERT_MSG(false,
            "FreeListAllocator: out of memory. Requested {} bytes.", size);
        return nullptr;
    }

    void Deallocate(void* ptr) override {
        if (!ptr) return;
        BlockHeader* block = reinterpret_cast<BlockHeader*>(
            reinterpret_cast<u8*>(ptr) - sizeof(BlockHeader));
        SHAPE_ASSERT_MSG(!block->free, "FreeListAllocator: double-free detected.");
        block->free = true;
        coalesce();
    }

    void Reset() override {
        BlockHeader* first = header_at(0);
        first->size = m_Capacity - sizeof(BlockHeader);
        first->free = true;
        first->next = nullptr;
    }

    usize Capacity() const { return m_Capacity; }

    /// Count free bytes (linear scan — for diagnostics only)
    usize FreeBytes() const {
        usize total = 0;
        const BlockHeader* current = header_at(0);
        while (current) {
            if (current->free) total += current->size;
            current = current->next;
        }
        return total;
    }

private:
    struct BlockHeader {
        usize         size; // payload size (bytes after header)
        bool          free;
        BlockHeader*  next;
    };

    BlockHeader* header_at(usize byteOffset) const {
        return reinterpret_cast<BlockHeader*>(m_Buffer.get() + byteOffset);
    }

    /// Split a free block into [size] + remainder if remainder is big enough.
    void split(BlockHeader* block, usize size) {
        usize remainder = block->size - size - sizeof(BlockHeader);
        if (remainder > sizeof(BlockHeader) + 8) { // only split if useful
            BlockHeader* newBlock = reinterpret_cast<BlockHeader*>(
                reinterpret_cast<u8*>(block) + sizeof(BlockHeader) + size);
            newBlock->size = remainder;
            newBlock->free = true;
            newBlock->next = block->next;

            block->size = size;
            block->next = newBlock;
        }
    }

    /// Merge consecutive free blocks (forward pass).
    void coalesce() {
        BlockHeader* current = header_at(0);
        while (current && current->next) {
            if (current->free && current->next->free) {
                current->size += sizeof(BlockHeader) + current->next->size;
                current->next  = current->next->next;
            } else {
                current = current->next;
            }
        }
    }

    std::unique_ptr<u8[]> m_Buffer;
    usize                 m_Capacity;
};

} // namespace Shape
