#include <catch2/catch_test_macros.hpp>
#include "Memory/ArenaAllocator.hpp"
#include "Memory/PoolAllocator.hpp"

struct TestStruct {
    i32 a;
    f32 b;
    char c[16];
    
    TestStruct() : a(10), b(20.0f) {
        c[0] = 'A';
    }
};

TEST_CASE("Arena Allocator functionality", "[Memory]") {
    Shape::ArenaAllocator arena(1024);

    SECTION("Linear Allocations and alignment") {
        void* ptr1 = arena.Allocate(10, 4);
        REQUIRE(ptr1 != nullptr);
        REQUIRE((reinterpret_cast<uintptr_t>(ptr1) % 4) == 0);

        void* ptr2 = arena.Allocate(23, 8);
        REQUIRE(ptr2 != nullptr);
        REQUIRE((reinterpret_cast<uintptr_t>(ptr2) % 8) == 0);

        // Make sure memory does not overlap
        REQUIRE(reinterpret_cast<uintptr_t>(ptr2) >= reinterpret_cast<uintptr_t>(ptr1) + 10);
    }

    SECTION("Arena Reset and Reuse") {
        void* ptr1 = arena.Allocate(100);
        usize used = arena.GetUsedMemory();
        REQUIRE(used >= 100);

        arena.Reset();
        REQUIRE(arena.GetUsedMemory() == 0);

        void* ptr2 = arena.Allocate(100);
        REQUIRE(ptr2 == ptr1); // Memory address is reused
    }

    SECTION("Arena OOM check") {
        void* ptr1 = arena.Allocate(1000);
        REQUIRE(ptr1 != nullptr);

        void* ptr2 = arena.Allocate(100); // Exceeds 1024 capacity due to alignment/overhead
        REQUIRE(ptr2 == nullptr);
    }
}

TEST_CASE("Pool Allocator functionality", "[Memory]") {
    SECTION("Normal operations") {
        const usize count = 4;
        Shape::PoolAllocator pool(sizeof(TestStruct), alignof(TestStruct), count);

        REQUIRE(pool.GetFreeCount() == count);

        TestStruct* obj1 = Shape::New<TestStruct>(pool);
        REQUIRE(obj1 != nullptr);
        REQUIRE(pool.GetFreeCount() == count - 1);
        REQUIRE(obj1->a == 10);
        REQUIRE(obj1->b == 20.0f);
        REQUIRE(obj1->c[0] == 'A');

        TestStruct* obj2 = Shape::New<TestStruct>(pool);
        TestStruct* obj3 = Shape::New<TestStruct>(pool);
        TestStruct* obj4 = Shape::New<TestStruct>(pool);

        REQUIRE(obj2 != nullptr);
        REQUIRE(obj3 != nullptr);
        REQUIRE(obj4 != nullptr);

        REQUIRE(pool.GetFreeCount() == 0);

        // OOM check
        void* overflow = pool.Allocate(sizeof(TestStruct), alignof(TestStruct));
        REQUIRE(overflow == nullptr);

        // Recycle obj2
        Shape::Delete<TestStruct>(pool, obj2);
        REQUIRE(pool.GetFreeCount() == 1);

        // Allocate again, should reuse obj2's block
        TestStruct* obj5 = Shape::New<TestStruct>(pool);
        REQUIRE(obj5 == obj2);
        REQUIRE(pool.GetFreeCount() == 0);
    }
}
