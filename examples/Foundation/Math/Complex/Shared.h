#ifndef FOUNDATION_EXAMPLES_MATH_COMPLEX_SHARED_H
#define FOUNDATION_EXAMPLES_MATH_COMPLEX_SHARED_H

#include <Foundation/Math/Complex.h>

namespace FoundationExamples {
namespace Math {
namespace Complex {

    struct Result {
        Foundation::Math::Complex<float> First;
        Foundation::Math::Complex<float> Second;
        Foundation::Math::Complex<float> Sum;
        Foundation::Math::Complex<float> Product;
        Foundation::Math::Complex<float> Conjugate;
        float MagnitudeSquared;
    };

    Result Run();

} // namespace Complex
} // namespace Math
} // namespace FoundationExamples

#endif
