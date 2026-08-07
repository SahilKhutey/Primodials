#include "Math/Matrix4.hpp"
#include "Core/Logger.hpp"

namespace Shape::Math {

Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 result;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            result.m[col][row] = 
                m[0][row] * other.m[col][0] +
                m[1][row] * other.m[col][1] +
                m[2][row] * other.m[col][2] +
                m[3][row] * other.m[col][3];
        }
    }
    return result;
}

Vector3 Matrix4::MultiplyPoint(const Vector3& point) const {
    f32 x = m[0][0] * point.x + m[1][0] * point.y + m[2][0] * point.z + m[3][0];
    f32 y = m[0][1] * point.x + m[1][1] * point.y + m[2][1] * point.z + m[3][1];
    f32 z = m[0][2] * point.x + m[1][2] * point.y + m[2][2] * point.z + m[3][2];
    f32 w = m[0][3] * point.x + m[1][3] * point.y + m[2][3] * point.z + m[3][3];
    
    if (Abs(w) > EPSILON && !NearEquals(w, 1.0f)) {
        f32 invW = 1.0f / w;
        return Vector3(x * invW, y * invW, z * invW);
    }
    return Vector3(x, y, z);
}

Vector3 Matrix4::MultiplyDirection(const Vector3& dir) const {
    f32 x = m[0][0] * dir.x + m[1][0] * dir.y + m[2][0] * dir.z;
    f32 y = m[0][1] * dir.x + m[1][1] * dir.y + m[2][1] * dir.z;
    f32 z = m[0][2] * dir.x + m[1][2] * dir.y + m[2][2] * dir.z;
    return Vector3(x, y, z);
}

Matrix4 Matrix4::Transposed() const {
    Matrix4 result;
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r) {
            result.m[c][r] = m[r][c];
        }
    }
    return result;
}

void Matrix4::Transpose() {
    *this = Transposed();
}

f32 Matrix4::Determinant() const {
    f32 s0 = m[0][0] * m[1][1] - m[1][0] * m[0][1];
    f32 s1 = m[0][0] * m[2][1] - m[2][0] * m[0][1];
    f32 s2 = m[0][0] * m[3][1] - m[3][0] * m[0][1];
    f32 s3 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
    f32 s4 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
    f32 s5 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

    f32 c5 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
    f32 c4 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
    f32 c3 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
    f32 c2 = m[0][2] * m[3][3] - m[3][2] * m[0][3];
    f32 c1 = m[0][2] * m[2][3] - m[2][2] * m[0][3];
    f32 c0 = m[0][2] * m[1][3] - m[1][2] * m[0][3];

    return s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
}

Matrix4 Matrix4::Inverted() const {
    f32 s0 = m[0][0] * m[1][1] - m[1][0] * m[0][1];
    f32 s1 = m[0][0] * m[2][1] - m[2][0] * m[0][1];
    f32 s2 = m[0][0] * m[3][1] - m[3][0] * m[0][1];
    f32 s3 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
    f32 s4 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
    f32 s5 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

    f32 c5 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
    f32 c4 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
    f32 c3 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
    f32 c2 = m[0][2] * m[3][3] - m[3][2] * m[0][3];
    f32 c1 = m[0][2] * m[2][3] - m[2][2] * m[0][3];
    f32 c0 = m[0][2] * m[1][3] - m[1][2] * m[0][3];

    f32 det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;

    if (Abs(det) <= EPSILON) {
        SHAPE_LOG_WARN("Matrix4 has determinant close to 0 (det={}), returning identity as fallback", det);
        return Identity();
    }

    f32 invDet = 1.0f / det;

    Matrix4 result;
    result.m[0][0] = (m[1][1] * c5 - m[2][1] * c4 + m[3][1] * c3) * invDet;
    result.m[0][1] = (-m[0][1] * c5 + m[2][1] * c2 - m[3][1] * c1) * invDet;
    result.m[0][2] = (m[0][1] * c4 - m[1][1] * c2 + m[3][1] * c0) * invDet;
    result.m[0][3] = (-m[0][1] * c3 + m[1][1] * c1 - m[2][1] * c0) * invDet;

    result.m[1][0] = (-m[1][0] * c5 + m[2][0] * c4 - m[3][0] * c3) * invDet;
    result.m[1][1] = (m[0][0] * c5 - m[2][0] * c2 + m[3][0] * c1) * invDet;
    result.m[1][2] = (-m[0][0] * c4 + m[1][0] * c2 - m[3][0] * c0) * invDet;
    result.m[1][3] = (m[0][0] * c3 - m[1][0] * c1 + m[2][0] * c0) * invDet;

    result.m[2][0] = (m[1][3] * s5 - m[2][3] * s4 + m[3][3] * s3) * invDet;
    result.m[2][1] = (-m[0][3] * s5 + m[2][3] * s2 - m[3][3] * s1) * invDet;
    result.m[2][2] = (m[0][3] * s4 - m[1][3] * s2 + m[3][3] * s0) * invDet;
    result.m[2][3] = (-m[0][3] * s3 + m[1][3] * s1 - m[2][3] * s0) * invDet;

    result.m[3][0] = (-m[1][2] * s5 + m[2][2] * s4 - m[3][2] * s3) * invDet;
    result.m[3][1] = (m[0][2] * s5 - m[2][2] * s2 + m[3][2] * s1) * invDet;
    result.m[3][2] = (-m[0][2] * s4 + m[1][2] * s2 - m[3][2] * s0) * invDet;
    result.m[3][3] = (m[0][2] * s3 - m[1][2] * s1 + m[2][2] * s0) * invDet;

    return result;
}

void Matrix4::Invert() {
    *this = Inverted();
}

Matrix4 Matrix4::Translation(const Vector3& translation) {
    Matrix4 result = Identity();
    result.m[3][0] = translation.x;
    result.m[3][1] = translation.y;
    result.m[3][2] = translation.z;
    return result;
}

Matrix4 Matrix4::Scaling(const Vector3& scale) {
    Matrix4 result = Identity();
    result.m[0][0] = scale.x;
    result.m[1][1] = scale.y;
    result.m[2][2] = scale.z;
    return result;
}

Matrix4 Matrix4::Rotation(f32 angleRadians, const Vector3& axis) {
    Matrix4 result = Identity();
    f32 c = Cos(angleRadians);
    f32 s = Sin(angleRadians);
    f32 t = 1.0f - c;

    Vector3 a = axis.Normalized();

    result.m[0][0] = t * a.x * a.x + c;
    result.m[0][1] = t * a.x * a.y + s * a.z;
    result.m[0][2] = t * a.x * a.z - s * a.y;

    result.m[1][0] = t * a.x * a.y - s * a.z;
    result.m[1][1] = t * a.y * a.y + c;
    result.m[1][2] = t * a.y * a.z + s * a.x;

    result.m[2][0] = t * a.x * a.z + s * a.y;
    result.m[2][1] = t * a.y * a.z - s * a.x;
    result.m[2][2] = t * a.z * a.z + c;

    return result;
}

Matrix4 Matrix4::Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 nearPlane, f32 farPlane) {
    Matrix4 result = Identity();
    result.m[0][0] = 2.0f / (right - left);
    result.m[1][1] = 2.0f / (top - bottom);
    result.m[2][2] = -2.0f / (farPlane - nearPlane);
    result.m[3][0] = -(right + left) / (right - left);
    result.m[3][1] = -(top + bottom) / (top - bottom);
    result.m[3][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    return result;
}

Matrix4 Matrix4::Perspective(f32 fovRadians, f32 aspect, f32 nearPlane, f32 farPlane) {
    Matrix4 result;
    f32 tanHalfFov = Tan(fovRadians * 0.5f);
    result.m[0][0] = 1.0f / (aspect * tanHalfFov);
    result.m[1][1] = 1.0f / tanHalfFov;
    result.m[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    result.m[2][3] = -1.0f;
    result.m[3][2] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
    return result;
}

Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
    Vector3 f = (target - eye).Normalized();
    Vector3 s = f.Cross(up).Normalized();
    Vector3 u = s.Cross(f);

    Matrix4 result = Identity();
    result.m[0][0] = s.x;
    result.m[1][0] = s.y;
    result.m[2][0] = s.z;
    result.m[0][1] = u.x;
    result.m[1][1] = u.y;
    result.m[2][1] = u.z;
    result.m[0][2] = -f.x;
    result.m[1][2] = -f.y;
    result.m[2][2] = -f.z;
    result.m[3][0] = -s.Dot(eye);
    result.m[3][1] = -u.Dot(eye);
    result.m[3][2] = f.Dot(eye);
    return result;
}

} // namespace Shape::Math
