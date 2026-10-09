#ifndef FOUNDATION_EXAMPLES_MATH_ARITHMETIC_SHARED_H
#define FOUNDATION_EXAMPLES_MATH_ARITHMETIC_SHARED_H

#include <stdint.h>

#include <Foundation/Math/Arithmetic.h>
#include <Foundation/Math/Trigonometry.h>

namespace FoundationExamples {
namespace Math {
namespace Arithmetic {

    struct Result {
        uint32_t GreatestCommonDivisor;
        float HalfTurnRadians;
        float QuarterTurnDegrees;
    };

    Result Run();

} // namespace Arithmetic
} // namespace Math
} // namespace FoundationExamples

#endif
