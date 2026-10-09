#include <iostream>

#include "Shared.h"

namespace {

    void PrintComplex(const char* name, const Foundation::Math::Complex<float>& value) {
        std::cout << name << ": " << value.real;
        std::cout << (value.imag < 0.0f ? " - " : " + ");
        std::cout << (value.imag < 0.0f ? -value.imag : value.imag) << "i\n";
    }

}

int main() {
    const FoundationExamples::Math::Complex::Result result =
        FoundationExamples::Math::Complex::Run();

    std::cout
        << "============================================================\n"
        << " FOUNDATION :: Math / Complex\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Demonstrate arithmetic and derived values for complex\n"
        << "  numbers.\n\n"
        << "[1] OPERANDS\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    Foundation::Math::Complex<float> a(3.0f, 4.0f);\n"
        << "    Foundation::Math::Complex<float> b(1.0f, -2.0f);\n";
    PrintComplex("  a ...................", result.First);
    PrintComplex("  b ...................", result.Second);
    std::cout
        << "\n[2] OPERATIONS\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    a + b;\n"
        << "    a * b;\n"
        << "    a.Conjugate();\n"
        << "    a.MagnitudeSquared();\n";
    PrintComplex("  a + b ...............", result.Sum);
    PrintComplex("  a * b ...............", result.Product);
    PrintComplex("  conjugate(a) ........", result.Conjugate);
    std::cout
        << "  magnitudeSquared(a) . " << result.MagnitudeSquared << '\n'
        << "  Meaning ............. 3^2 + 4^2; no square root needed\n\n"
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  Complex keeps real/imaginary arithmetic explicit and\n"
        << "  provides common derived operations.\n"
        << "============================================================\n";

    return result.Sum.real == 4.0f
        && result.Product.real == 11.0f
        && result.MagnitudeSquared == 25.0f
        ? 0
        : 1;
}
