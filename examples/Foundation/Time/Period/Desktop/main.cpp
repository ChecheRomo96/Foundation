#include <iostream>

#include "Shared.h"

namespace {

    void PrintRatio(
        const char* name,
        const char* code,
        const Foundation::Math::UnsignedRatio& ratio
    ) {
        std::cout << name << '\n';
        std::cout << "------------------------------------------------------------\n";
        std::cout << "  CODE\n";
        std::cout << "    " << code << '\n';
        std::cout << "  Exact ratio .......... " << ratio.Num() << " / " << ratio.Den() << '\n';
        std::cout << "  Valid ................ " << (ratio.IsValid() ? "yes" : "no") << '\n';
        std::cout << "  Decimal view ......... " << ratio.ToFloat() << "\n\n";
    }

    void PrintPeriod(
        const char* name,
        const char* code,
        const Foundation::Time::Period& period
    ) {
        std::cout << name << '\n';
        std::cout << "------------------------------------------------------------\n";
        std::cout << "  CODE\n";
        std::cout << "    " << code << '\n';
        std::cout << "  Exact period ......... " << period.Num()
                  << " / " << period.Den() << " seconds per tick\n";
        std::cout << "  Valid ................ " << (period.IsValid() ? "yes" : "no") << '\n';
        std::cout << "  Seconds/tick ......... " << period.Seconds() << " s\n";
        std::cout << "  Milliseconds/tick .... " << period.Milliseconds() << " ms\n";
        std::cout << "  Microseconds/tick .... " << period.Microseconds() << " us\n\n";
    }

}

int main() {
    std::cout
        << "============================================================\n"
        << " FOUNDATION :: Time / Period\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Represent seconds-per-tick exactly, derive rates, mutate\n"
        << "  values, and expose invalid input.\n\n";

    FoundationExamples::Time::Period::Run(PrintRatio, PrintPeriod);

    std::cout
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  Period uses an exact unsigned ratio. Both terms must be\n"
        << "  non-zero before a reciprocal Frequency exists.\n"
        << "============================================================\n";
    return 0;
}
