#pragma once

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"
#include <new>
#include <utility>

namespace Shape {

class Allocator {
public:
    virtual ~Allocator() = default;

    virtual void* Allocate(usize size, usize alignment = alignof(std::max_align_t)) = 0;
    virtual void Deallocate(void* ptr) = 0;
    virtual void Reset() {}
};

// Global helpers for custom allocation
template <typename T, typename... Args>
SHAPE_INLINE T* New(Allocator& allocator, Args&&... args) {
    void* memory = allocator.Allocate(sizeof(T), alignof(T));
    SHAPE_ASSERT(memory != nullptr);
    return new(memory) T(std::forward<Args>(args)...);
}

template <typename T>
SHAPE_INLINE void Delete(Allocator& allocator, T* ptr) {
    if (ptr) {
        ptr->~T();
        allocator.Deallocate(ptr);
    }
}

// Allocator helper for contiguous arrays
template <typename T>
SHAPE_INLINE T* NewArray(Allocator& allocator, usize count) {
    if (count == 0) return nullptr;
    // We allocate extra memory to store the size of the array for proper destruction if it contains non-trivial objects.
    // However, to keep it simple, if T is trivially destructible, we don't need array overhead.
    // Let's implement a robust version that handles non-trivial types.
    constexpr usize alignment = alignof(T);
    const usize arraySize = sizeof(T) * count;
    
    void* memory = allocator.Allocate(arraySize, alignment);
    SHAPE_ASSERT(memory != nullptr);
    
    T* arrayPtr = static_cast<T*>(memory);
    for (usize i = 0; i < count; ++i) {
        new (&arrayPtr[i]) T();
    }
    return arrayPtr;
}

template <typename T>
SHAPE_INLINE void DeleteArray(Allocator& allocator, T* ptr, usize count) {
    if (ptr) {
        for (usize i = count; i > 0; --i) {
            ptr[i - 1].~T();
        }
        allocator.Deallocate(ptr);
    }
}

// Utility to align a value to the next multiple of alignment
SHAPE_INLINE uintptr_t AlignAddress(uintptr_t address, usize alignment) {
    SHAPE_ASSERT((alignment & (alignment - 1)) == 0); // Alignment must be power of 2
    return (address + (alignment - 1)) & ~(alignment - 1);
}

} // namespace Shape
