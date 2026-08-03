#include "ShapeEngine/Math/Matrix2.hpp"

namespace Shape::Math {

// Static compilation checkpoint function
void Matrix2CompilationCheck() {
    Matrix2f m = Matrix2f::Identity();
    (void)Determinant(m);
}

} // namespace Shape::Math
