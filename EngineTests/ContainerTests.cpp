/// @file ContainerTests.cpp
/// @brief Test suite for all containers in Engine/Containers/.
///
/// Covers: Array, Vector, SmallVector, HashMap, Bitset, RingBuffer, Pool,
///         Stack, Queue.
///
/// DETERMINISM: All tests use fixed seeds and deterministic inputs.

#include <catch2/catch_test_macros.hpp>
#include "Containers/Array.hpp"
#include "Containers/Vector.hpp"
#include "Containers/SmallVector.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/Bitset.hpp"
#include "Containers/RingBuffer.hpp"
#include "Containers/Pool.hpp"
#include "Containers/Stack.hpp"
#include "Containers/Queue.hpp"

using namespace Shape;

// ─── Array ────────────────────────────────────────────────────────────────────

TEST_CASE("Array: size and capacity", "[containers][array]") {
    Array<int, 4> a;
    REQUIRE(a.Size() == 4);
    REQUIRE(a.Capacity() == 4);
    REQUIRE_FALSE(a.Empty());
}

TEST_CASE("Array: element access", "[containers][array]") {
    Array<int, 4> a;
    a[0] = 10; a[1] = 20; a[2] = 30; a[3] = 40;
    REQUIRE(a[0] == 10);
    REQUIRE(a.Front() == 10);
    REQUIRE(a.Back() == 40);
}

TEST_CASE("Array: fill", "[containers][array]") {
    Array<int, 8> a;
    a.Fill(7);
    for (usize i = 0; i < 8; ++i) REQUIRE(a[i] == 7);
}

TEST_CASE("Array: range-for iteration", "[containers][array]") {
    Array<int, 4> a;
    a.Fill(3);
    int sum = 0;
    for (auto v : a) sum += v;
    REQUIRE(sum == 12);
}

TEST_CASE("Array: equality", "[containers][array]") {
    Array<int, 3> a, b;
    a.Fill(1); b.Fill(1);
    REQUIRE(a == b);
    b[1] = 99;
    REQUIRE(a != b);
}

// ─── Vector ───────────────────────────────────────────────────────────────────

TEST_CASE("Vector: push and pop", "[containers][vector]") {
    Vector<int> v;
    v.PushBack(1);
    v.PushBack(2);
    v.PushBack(3);
    REQUIRE(v.Size() == 3);
    REQUIRE(v.Back() == 3);
    v.PopBack();
    REQUIRE(v.Size() == 2);
    REQUIRE(v.Back() == 2);
}

TEST_CASE("Vector: reserve and resize", "[containers][vector]") {
    Vector<int> v;
    v.Reserve(100);
    REQUIRE(v.Capacity() >= 100);
    REQUIRE(v.Size() == 0);
    v.Resize(10);
    REQUIRE(v.Size() == 10);
}

TEST_CASE("Vector: emplace back", "[containers][vector]") {
    struct Point { int x, y; };
    Vector<Point> v;
    v.EmplaceBack(1, 2);
    v.EmplaceBack(3, 4);
    REQUIRE(v.Size() == 2);
    REQUIRE(v[0].x == 1);
    REQUIRE(v[1].y == 4);
}

TEST_CASE("Vector: swap erase (O(1), unordered)", "[containers][vector]") {
    Vector<int> v;
    for (int i = 0; i < 5; ++i) v.PushBack(i); // [0,1,2,3,4]
    v.SwapErase(1); // Replaces index 1 with 4 → [0,4,2,3]
    REQUIRE(v.Size() == 4);
    // Verify 1 is gone
    bool found1 = false;
    for (auto x : v) if (x == 1) found1 = true;
    REQUIRE_FALSE(found1);
}

TEST_CASE("Vector: order-preserving erase", "[containers][vector]") {
    Vector<int> v;
    for (int i = 0; i < 5; ++i) v.PushBack(i); // [0,1,2,3,4]
    v.Erase(2); // → [0,1,3,4]
    REQUIRE(v.Size() == 4);
    REQUIRE(v[0] == 0);
    REQUIRE(v[1] == 1);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
}

TEST_CASE("Vector: growth beyond initial capacity", "[containers][vector]") {
    Vector<int> v;
    for (int i = 0; i < 1000; ++i) v.PushBack(i);
    REQUIRE(v.Size() == 1000);
    REQUIRE(v[999] == 999);
}

TEST_CASE("Vector: copy and move semantics", "[containers][vector]") {
    Vector<int> a;
    a.PushBack(1); a.PushBack(2); a.PushBack(3);

    Vector<int> b = a;
    REQUIRE(b.Size() == 3);
    REQUIRE(b[0] == 1);

    Vector<int> c = std::move(a);
    REQUIRE(c.Size() == 3);
    REQUIRE(a.Empty()); // Moved-from is empty
}

// ─── SmallVector ──────────────────────────────────────────────────────────────

TEST_CASE("SmallVector: inline storage (no heap)", "[containers][smallvec]") {
    SmallVector<int, 8> sv;
    for (int i = 0; i < 8; ++i) sv.PushBack(i);
    REQUIRE(sv.IsInline());
    REQUIRE(sv.Size() == 8);
}

TEST_CASE("SmallVector: heap fallback when inline full", "[containers][smallvec]") {
    SmallVector<int, 4> sv;
    for (int i = 0; i < 10; ++i) sv.PushBack(i);
    REQUIRE_FALSE(sv.IsInline());
    REQUIRE(sv.Size() == 10);
    REQUIRE(sv[9] == 9);
}

TEST_CASE("SmallVector: swap erase", "[containers][smallvec]") {
    SmallVector<int, 8> sv;
    sv.PushBack(10); sv.PushBack(20); sv.PushBack(30);
    sv.SwapErase(0);
    REQUIRE(sv.Size() == 2);
    REQUIRE(sv[0] == 30); // 30 moved to position 0
}

// ─── HashMap ──────────────────────────────────────────────────────────────────

TEST_CASE("HashMap: insert and find", "[containers][hashmap]") {
    HashMap<int, int> m;
    m.Insert(1, 100);
    m.Insert(2, 200);
    m.Insert(3, 300);
    REQUIRE(m.Size() == 3);
    REQUIRE(*m.Find(1) == 100);
    REQUIRE(*m.Find(2) == 200);
    REQUIRE(m.Find(99) == nullptr);
}

TEST_CASE("HashMap: contains", "[containers][hashmap]") {
    HashMap<int, int> m;
    m.Insert(42, 7);
    REQUIRE(m.Contains(42));
    REQUIRE_FALSE(m.Contains(1));
}

TEST_CASE("HashMap: update existing key", "[containers][hashmap]") {
    HashMap<int, int> m;
    m.Insert(5, 10);
    m.Insert(5, 20); // Update
    REQUIRE(*m.Find(5) == 20);
}

TEST_CASE("HashMap: remove", "[containers][hashmap]") {
    HashMap<int, int> m;
    m.Insert(1, 1); m.Insert(2, 2); m.Insert(3, 3);
    REQUIRE(m.Remove(2));
    REQUIRE_FALSE(m.Contains(2));
    REQUIRE(m.Size() == 2);
}

TEST_CASE("HashMap: operator[]", "[containers][hashmap]") {
    HashMap<int, int> m;
    m[10] = 99;
    REQUIRE(m.Contains(10));
    REQUIRE(*m.Find(10) == 99);
}

TEST_CASE("HashMap: grow beyond initial capacity", "[containers][hashmap]") {
    HashMap<int, int> m;
    for (int i = 0; i < 200; ++i) m.Insert(i, i * 2);
    REQUIRE(m.Size() == 200);
    for (int i = 0; i < 200; ++i) {
        REQUIRE(m.Contains(i));
        REQUIRE(*m.Find(i) == i * 2);
    }
}

// ─── Bitset ───────────────────────────────────────────────────────────────────

TEST_CASE("Bitset: set and test", "[containers][bitset]") {
    Bitset<128> bs;
    REQUIRE(bs.None());
    bs.Set(0);
    bs.Set(63);
    bs.Set(127);
    REQUIRE(bs.Test(0));
    REQUIRE(bs.Test(63));
    REQUIRE(bs.Test(127));
    REQUIRE_FALSE(bs.Test(1));
}

TEST_CASE("Bitset: clear bit", "[containers][bitset]") {
    Bitset<64> bs;
    bs.Set(5);
    REQUIRE(bs.Test(5));
    bs.Clear(5);
    REQUIRE_FALSE(bs.Test(5));
}

TEST_CASE("Bitset: count", "[containers][bitset]") {
    Bitset<64> bs;
    bs.Set(0); bs.Set(1); bs.Set(63);
    REQUIRE(bs.Count() == 3);
}

TEST_CASE("Bitset: set-bit iteration", "[containers][bitset]") {
    Bitset<256> bs;
    bs.Set(3); bs.Set(7); bs.Set(128);
    int found = 0;
    for (usize bit : bs) {
        REQUIRE((bit == 3 || bit == 7 || bit == 128));
        ++found;
    }
    REQUIRE(found == 3);
}

TEST_CASE("Bitset: SetAll and All()", "[containers][bitset]") {
    Bitset<64> bs;
    bs.SetAll();
    REQUIRE(bs.All());
    REQUIRE(bs.Count() == 64);
}

TEST_CASE("Bitset: bitwise AND", "[containers][bitset]") {
    Bitset<64> a, b;
    a.Set(5); a.Set(10);
    b.Set(5); b.Set(20);
    auto c = a & b;
    REQUIRE(c.Test(5));
    REQUIRE_FALSE(c.Test(10));
    REQUIRE_FALSE(c.Test(20));
}

// ─── RingBuffer ───────────────────────────────────────────────────────────────

TEST_CASE("RingBuffer: push and pop", "[containers][ring]") {
    RingBuffer<int, 8> rb;
    REQUIRE(rb.Empty());
    rb.Push(1); rb.Push(2); rb.Push(3);
    REQUIRE(rb.Size() == 3);
    int out;
    REQUIRE(rb.Pop(out));
    REQUIRE(out == 1);
    REQUIRE(rb.Pop(out));
    REQUIRE(out == 2);
}

TEST_CASE("RingBuffer: full detection", "[containers][ring]") {
    RingBuffer<int, 4> rb;
    REQUIRE(rb.Push(1)); REQUIRE(rb.Push(2));
    REQUIRE(rb.Push(3)); REQUIRE(rb.Push(4));
    REQUIRE(rb.Full());
    REQUIRE_FALSE(rb.Push(5)); // Should reject when full
}

TEST_CASE("RingBuffer: wrap around", "[containers][ring]") {
    RingBuffer<int, 4> rb;
    rb.Push(1); rb.Push(2);
    int out;
    rb.Pop(out); rb.Pop(out); // Drain two
    rb.Push(3); rb.Push(4); rb.Push(5); // Fill after wrap
    REQUIRE(rb.Size() == 3);
    REQUIRE(rb.Pop(out)); REQUIRE(out == 3);
}

// ─── Pool ─────────────────────────────────────────────────────────────────────

struct TestObj { int value = 0; };

TEST_CASE("Pool: acquire and release", "[containers][pool]") {
    Pool<TestObj, 16> pool;
    REQUIRE(pool.Count() == 0);
    u32 id = pool.Acquire();
    REQUIRE(id != Pool<TestObj, 16>::INVALID);
    REQUIRE(pool.Count() == 1);
    pool.Get(id).value = 42;
    REQUIRE(pool.Get(id).value == 42);
    pool.Release(id);
    REQUIRE(pool.Count() == 0);
}

TEST_CASE("Pool: full detection", "[containers][pool]") {
    Pool<TestObj, 4> pool;
    for (int i = 0; i < 4; ++i) pool.Acquire();
    REQUIRE(pool.Full());
    REQUIRE(pool.Acquire() == Pool<TestObj, 4>::INVALID);
}

TEST_CASE("Pool: reuse of released slots", "[containers][pool]") {
    Pool<TestObj, 4> pool;
    u32 a = pool.Acquire();
    u32 b = pool.Acquire();
    pool.Release(a);
    u32 c = pool.Acquire(); // Should reuse slot a
    REQUIRE(c == a);
    pool.Release(b);
    pool.Release(c);
    REQUIRE(pool.Count() == 0);
}

// ─── Stack ────────────────────────────────────────────────────────────────────

TEST_CASE("Stack: push, top, pop", "[containers][stack]") {
    Stack<int, 8> s;
    REQUIRE(s.Empty());
    s.Push(1); s.Push(2); s.Push(3);
    REQUIRE(s.Size() == 3);
    REQUIRE(s.Top() == 3);
    s.Pop();
    REQUIRE(s.Top() == 2);
    REQUIRE(s.Size() == 2);
}

TEST_CASE("Stack: LIFO order", "[containers][stack]") {
    Stack<int, 8> s;
    for (int i = 0; i < 5; ++i) s.Push(i);
    for (int i = 4; i >= 0; --i) {
        REQUIRE(s.Top() == i);
        s.Pop();
    }
    REQUIRE(s.Empty());
}

// ─── Queue ────────────────────────────────────────────────────────────────────

TEST_CASE("Queue: enqueue and dequeue", "[containers][queue]") {
    Queue<int, 8> q;
    REQUIRE(q.Empty());
    q.Enqueue(1); q.Enqueue(2); q.Enqueue(3);
    REQUIRE(q.Size() == 3);
    REQUIRE(q.Front() == 1);
    REQUIRE(q.Dequeue() == 1);
    REQUIRE(q.Dequeue() == 2);
    REQUIRE(q.Size() == 1);
}

TEST_CASE("Queue: FIFO order", "[containers][queue]") {
    Queue<int, 8> q;
    for (int i = 0; i < 5; ++i) q.Enqueue(i);
    for (int i = 0; i < 5; ++i) {
        REQUIRE(q.Dequeue() == i);
    }
    REQUIRE(q.Empty());
}

TEST_CASE("Queue: wrap around", "[containers][queue]") {
    Queue<int, 4> q;
    q.Enqueue(1); q.Enqueue(2);
    q.Dequeue(); q.Dequeue();
    q.Enqueue(3); q.Enqueue(4); q.Enqueue(5);
    REQUIRE(q.Size() == 3);
    REQUIRE(q.Dequeue() == 3);
    REQUIRE(q.Dequeue() == 4);
    REQUIRE(q.Dequeue() == 5);
}
