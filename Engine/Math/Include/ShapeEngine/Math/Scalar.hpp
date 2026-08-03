#pragma once

#include <cmath>
#include <limits>
#include <type_traits>

namespace Shape::Math {

template<typename T>
class Scalar {
    static_assert(std::is_arithmetic_v<T>,
        "Scalar<T> requires arithmetic type.");

public:
    using ValueType = T;

    constexpr Scalar() = default;
    constexpr Scalar(T value) : m_value(value) {}

    constexpr operator T() const {
        return m_value;
    }

    constexpr T Value() const {
        return m_value;
    }

    static constexpr T Min() {
        return std::numeric_limits<T>::lowest();
    }

    static constexpr T Max() {
        return std::numeric_limits<T>::max();
    }

    static constexpr T Epsilon() {
        return std::numeric_limits<T>::epsilon();
    }

private:
    T m_value{};
};

} // namespace Shape::Math
