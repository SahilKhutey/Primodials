# ⚡ PRIMORDIALS — Physics & Spatial Query Optimizations

---

## 1. Zero-Allocation Spatial Hash Grid

- **Dense Bucket Layout:** Buckets use contiguous flat memory with fixed chunk capacities rather than node-based linked lists.
- **Bit-Packed Coordinates:** $32$-bit integer grid coords packed into a single $64$-bit integer key for instantaneous bitwise indexing.
- **SIMD AVX2 Range Queries:** Processes 8 candidate entities per instruction cycle via `_mm256_sub_ps` and `_mm256_mul_ps`.

---

## 2. Toroidal Coordinate Math Optimization

Replaces expensive modulo operations (`%`) in inner physics loops with branchless floating-point wrap logic:

```cpp
[[nodiscard]] constexpr float wrap_toroidal(float pos, float max_bound) noexcept {
    if (pos < 0.0f) return pos + max_bound;
    if (pos >= max_bound) return pos - max_bound;
    return pos;
}
```
