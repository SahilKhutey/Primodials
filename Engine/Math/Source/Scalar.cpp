#include "ShapeEngine/Math/Scalar.hpp"
#include "ShapeEngine/Math/MathFunctions.hpp"

namespace Shape::Math {

// Static compilation checkpoint function
void ScalarCompilationCheck() {
    Scalar<float> s(10.0f);
    (void)s.Value();
}

} // namespace Shape::Math
