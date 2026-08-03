#pragma once

/// @file NonCopyable.hpp
/// @brief CRTP base classes for non-copyable and non-movable types.
///
/// Usage:
///   class MyResource : public Shape::NonCopyable {
///       // copy constructor/assignment are deleted
///       // move is allowed
///   };

namespace Shape {

/// Base class that deletes copy operations but allows move.
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&)            = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&)                 = default;
    NonCopyable& operator=(NonCopyable&&)      = default;
protected:
    ~NonCopyable() = default;
};

/// Base class that deletes both copy and move operations.
class NonMovable {
public:
    NonMovable() = default;
    NonMovable(const NonMovable&)            = delete;
    NonMovable& operator=(const NonMovable&) = delete;
    NonMovable(NonMovable&&)                 = delete;
    NonMovable& operator=(NonMovable&&)      = delete;
protected:
    ~NonMovable() = default;
};

} // namespace Shape
