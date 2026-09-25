#include "Shared.h"

namespace FoundationExamples {
namespace Math {
namespace Complex {

    Result Run() {
        Result result;
        result.First = Foundation::Math::Complex<float>(3.0f, 4.0f);
        result.Second = Foundation::Math::Complex<float>(1.0f, -2.0f);
        result.Sum = result.First + result.Second;
        result.Product = result.First * result.Second;
        result.Conjugate = result.First.Conjugate();
        result.MagnitudeSquared = result.First.MagnitudeSquared();
        return result;
    }

} // namespace Complex
} // namespace Math
} // namespace FoundationExamples
