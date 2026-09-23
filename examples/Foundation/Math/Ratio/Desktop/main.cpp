#include <iostream>

#include "Shared.h"

namespace {

    void PrintRatio(
        const char* name,
        const char* code,
        const Foundation::Math::Ratio& ratio
    ) {
        std::cout << name << '\n';
        std::cout << "------------------------------------------------------------\n";
        std::cout << "  CODE\n";
        std::cout << "    " << code << '\n';
        std::cout << "  Exact value .......... " << ratio.Num() << " / " << ratio.Den() << '\n';
        std::cout << "  Valid denominator .... "
                  << (ratio.IsValid() ? "yes" : "no") << '\n';
        std::cout << "  Decimal view ......... " << ratio.ToFloat() << "\n\n";
    }

}

int main() {
    std::cout
        << "============================================================\n"
        << " FOUNDATION :: Math / Ratio\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Preserve exact numerator/denominator values and convert\n"
        << "  them to floating point only when needed.\n\n";

    FoundationExamples::Math::Ratio::Run(PrintRatio);

    std::cout
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  Ratio keeps values such as 30000/1001 exact instead of\n"
        << "  beginning with a rounded decimal.\n"
        << "============================================================\n";
    return 0;
}
