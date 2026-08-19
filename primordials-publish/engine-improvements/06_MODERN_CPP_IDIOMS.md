# 💎 PRIMORDIALS — Modern C++23 Idioms & Standards Enforcement

---

## 🛠️ C++23 LANGUAGE FEATURES UTILIZED

1. **Concepts & Constraints (`<concepts>`):** Type-safe component tagging ensuring zero-overhead template instantiation.
2. **`std::expected<T, E>`:** Monadic functional error handling eliminating exception overhead in hot loops.
3. **`std::span<T>`:** Zero-allocation contiguous buffer views across rendering and physics passes.
4. **`std::print` / `std::println` (`<print>`):** Type-safe, high-performance formatted output without iostream overhead.
5. **Explicit Branch Predictor Attributes (`[[likely]]` / `[[unlikely]]`):** Hinting hot paths to the compiler.
6. **`std::bit_cast` & `<bit>` Intrinsics:** Hardware-accelerated bit manipulation for hash keys and Morton codes.
