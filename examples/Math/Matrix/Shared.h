#ifndef FOUNDATION_EXAMPLES_MATH_MATRIX_SHARED_H
#define FOUNDATION_EXAMPLES_MATH_MATRIX_SHARED_H

#include <Foundation/Math/Matrix.h>

namespace FoundationExamples {
namespace Math {
namespace Matrix {

    struct Result {
        int First[2][2];
        int Second[2][2];
        int FixedProduct[2][2];
        int FixedTranspose[2][2];
        bool DynamicMultiplySucceeded;
        bool DynamicResultOwnsData;
        int DynamicProduct[2][2];
    };

    Result Run();

} // namespace Matrix
} // namespace Math
} // namespace FoundationExamples

#endif
