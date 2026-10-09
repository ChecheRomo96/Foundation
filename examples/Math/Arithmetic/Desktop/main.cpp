#include <iostream>

#include "Shared.h"

int main() {
    const FoundationExamples::Math::Arithmetic::Result result =
        FoundationExamples::Math::Arithmetic::Run();

    std::cout
        << "============================================================\n"
        << " FOUNDATION :: Math / Arithmetic\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Demonstrate integer GCD and angle conversions.\n\n"
        << "[1] GREATEST COMMON DIVISOR\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    Foundation::Math::GCD(84, 30);\n"
        << "  Input ............... 84 and 30\n"
        << "  Result .............. " << result.GreatestCommonDivisor << '\n'
        << "  Meaning ............. Largest integer dividing both inputs\n\n"
        << "[2] DEGREES TO RADIANS\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    Foundation::Math::DegToRad(180.0f);\n"
        << "  Result .............. " << result.HalfTurnRadians
        << " rad (approximately Pi)\n\n"
        << "[3] RADIANS TO DEGREES\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    Foundation::Math::RadToDeg(Foundation::Math::Pi / 2.0f);\n"
        << "  Result .............. " << result.QuarterTurnDegrees << " degrees\n\n"
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  Foundation provides the conversion and constant while the\n"
        << "  caller keeps the units explicit.\n"
        << "============================================================\n";

    return result.GreatestCommonDivisor == 6
        && result.QuarterTurnDegrees == 90.0f
        ? 0
        : 1;
}
