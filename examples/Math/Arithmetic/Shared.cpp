#include "Shared.h"

namespace FoundationExamples {
namespace Math {
namespace Arithmetic {

    Result Run() {
        Result result;
        result.GreatestCommonDivisor = Foundation::Math::GCD(84, 30);
        result.HalfTurnRadians = Foundation::Math::DegToRad(180.0f);
        result.QuarterTurnDegrees = Foundation::Math::RadToDeg(
            Foundation::Math::Pi / 2.0f
        );
        return result;
    }

} // namespace Arithmetic
} // namespace Math
} // namespace FoundationExamples
