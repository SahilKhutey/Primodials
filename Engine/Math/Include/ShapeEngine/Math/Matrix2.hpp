#pragma once

#include "Core/Platform.hpp"
#include "Math/MathUtils.hpp"
#include "ShapeEngine/Math/Angle.hpp"
#include "ShapeEngine/Math/Vector2.hpp"
#include <optional>
#include <cmath>

namespace Shape::Math {

// Forward Declaration of Matrix template
template<typename T, usize Rows, usize Cols>
struct Matrix;

// Specialization for 2x2 Matrix
template<typename T>
struct Matrix<T, 2, 2> {
    T m00 = 0, m01 = 0;
    T m10 = 0, m11 = 0;

    constexpr Matrix() : m00(0), m01(0), m10(0), m11(0) {}
    constexpr Matrix(T diagonal) : m00(diagonal), m01(0), m10(0), m11(diagonal) {}
    constexpr Matrix(T n00, T n01, T n10, T n11) : m00(n00), m01(n01), m10(n10), m11(n11) {}

    // Static Constants
    static constexpr Matrix Identity() {
        return {
            static_cast<T>(1), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(1)
        };
    }

    static constexpr Matrix Zero() {
        return {
            static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0)
        };
    }

    static Matrix Rotation(Radians angle) {
        T c = static_cast<T>(Cos(angle.value));
        T s = static_cast<T>(Sin(angle.value));
        return {
            c, -s,
            s,  c
        };
    }

    static constexpr Matrix Scale(T x, T y) {
        return {
            x, static_cast<T>(0),
            static_cast<T>(0), y
        };
    }

    // Accessors
    constexpr T& operator()(usize row, usize col) {
        if (row == 0 && col == 0) return m00;
        if (row == 0 && col == 1) return m01;
        if (row == 1 && col == 0) return m10;
        return m11;
    }

    constexpr const T& operator()(usize row, usize col) const {
        if (row == 0 && col == 0) return m00;
        if (row == 0 && col == 1) return m01;
        if (row == 1 && col == 0) return m10;
        return m11;
    }

    // Unary Operators
    constexpr bool operator==(const Matrix& o) const {
        return m00 == o.m00 && m01 == o.m01 && m10 == o.m10 && m11 == o.m11;
    }

    constexpr bool operator!=(const Matrix& o) const {
        return !(*this == o);
    }

    constexpr Matrix operator+(const Matrix& o) const {
        return {m00 + o.m00, m01 + o.m01, m10 + o.m10, m11 + o.m11};
    }

    constexpr Matrix operator-(const Matrix& o) const {
        return {m00 - o.m00, m01 - o.m01, m10 - o.m10, m11 - o.m11};
    }

    constexpr Matrix operator*(T scalar) const {
        return {m00 * scalar, m01 * scalar, m10 * scalar, m11 * scalar};
    }

    constexpr Matrix operator/(T scalar) const {
        return {m00 / scalar, m01 / scalar, m10 / scalar, m11 / scalar};
    }
};

template<typename T>
using Matrix2 = Matrix<T, 2, 2>;

using Matrix2f = Matrix2<float>;
using Matrix2d = Matrix2<double>;

// Matrix Multiplication
template<typename T>
constexpr Matrix2<T> operator*(const Matrix2<T>& A, const Matrix2<T>& B) {
    return {
        A.m00 * B.m00 + A.m01 * B.m10,
        A.m00 * B.m01 + A.m01 * B.m11,
        A.m10 * B.m00 + A.m11 * B.m10,
        A.m10 * B.m01 + A.m11 * B.m11
    };
}

// Matrix x Vector Multiplication
template<typename T>
constexpr Vector2f operator*(const Matrix2<T>& M, const Vector2f& V) {
    return {
        M.m00 * V.x + M.m01 * V.y,
        M.m10 * V.x + M.m11 * V.y
    };
}

// Algorithms
template<typename T>
constexpr T Determinant(const Matrix2<T>& M) {
    return M.m00 * M.m11 - M.m01 * M.m10;
}

template<typename T>
constexpr Matrix2<T> Transpose(const Matrix2<T>& M) {
    return {
        M.m00, M.m10,
        M.m01, M.m11
    };
}

template<typename T>
constexpr Matrix2<T> Adjoint(const Matrix2<T>& M) {
    return {
        M.m11, -M.m01,
        -M.m10, M.m00
    };
}

template<typename T>
constexpr std::optional<Matrix2<T>> SafeInverse(const Matrix2<T>& M, T epsilon = static_cast<T>(1e-6)) {
    T det = Determinant(M);
    if (Abs(det) <= epsilon) {
        return std::nullopt;
    }
    return Adjoint(M) * (static_cast<T>(1) / det);
}

template<typename T>
constexpr Matrix2<T> Inverse(const Matrix2<T>& M) {
    T det = Determinant(M);
    return Adjoint(M) * (static_cast<T>(1) / det);
}

// Traits
template<typename T>
constexpr bool IsIdentity(const Matrix2<T>& M, T epsilon = static_cast<T>(1e-6)) {
    return NearEquals(M.m00, static_cast<T>(1), epsilon) && NearEquals(M.m01, static_cast<T>(0), epsilon) &&
           NearEquals(M.m10, static_cast<T>(0), epsilon) && NearEquals(M.m11, static_cast<T>(1), epsilon);
}

template<typename T>
constexpr bool IsSymmetric(const Matrix2<T>& M, T epsilon = static_cast<T>(1e-6)) {
    return NearEquals(M.m01, M.m10, epsilon);
}

template<typename T>
constexpr bool IsOrthogonal(const Matrix2<T>& M, T epsilon = static_cast<T>(1e-6)) {
    return IsIdentity(M * Transpose(M), epsilon);
}

template<typename T>
constexpr bool IsInvertible(const Matrix2<T>& M, T epsilon = static_cast<T>(1e-6)) {
    return Abs(Determinant(M)) > epsilon;
}

template<typename T>
constexpr T Trace(const Matrix2<T>& M) {
    return M.m00 + M.m11;
}

// Decompositions
template<typename T>
inline Radians ExtractRotation(const Matrix2<T>& M) {
    return Radians(std::atan2(M.m10, M.m00));
}

template<typename T>
inline Vector2f ExtractScale(const Matrix2<T>& M) {
    return {
        Sqrt(M.m00 * M.m00 + M.m10 * M.m10),
        Sqrt(M.m01 * M.m01 + M.m11 * M.m11)
    };
}

} // namespace Shape::Math
