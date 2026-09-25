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
        std::cout << "  Exact ratio .......... " << ratio.Numerator() << " / " << ratio.Denominator() << '\n';
        std::cout << "  Valid ................ " << (ratio.IsValid() ? "yes" : "no") << '\n';
        std::cout << "  Decimal view ......... " << ratio.ToFloat() << "\n\n";
    }

    void PrintFrequency(
        const char* name,
        const char* code,
        const Foundation::Time::Frequency& frequency
    ) {
        std::cout << name << '\n';
        std::cout << "------------------------------------------------------------\n";
        std::cout << "  CODE\n";
        std::cout << "    " << code << '\n';
        std::cout << "  Exact frequency ...... " << frequency.Numerator()
                  << " / " << frequency.Denominator() << " ticks per second\n";
        std::cout << "  Valid ................ " << (frequency.IsValid() ? "yes" : "no") << '\n';
        std::cout << "  Rate ................. " << frequency.Hertz() << " Hz\n";
        std::cout << "  Seconds/tick ......... " << frequency.PeriodSeconds() << " s\n";
        std::cout << "  Milliseconds/tick .... "
                  << frequency.PeriodMilliseconds() << " ms\n";
        std::cout << "  Microseconds/tick .... "
                  << frequency.PeriodMicroseconds() << " us\n\n";
    }

}

int main() {
    std::cout
        << "============================================================\n"
        << " FOUNDATION :: Time / Frequency\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Represent ticks-per-second exactly, derive time per tick,\n"
        << "  mutate values, and expose invalid input.\n\n";

    FoundationExamples::Time::Frequency::Run(PrintRatio, PrintFrequency);

    std::cout
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  Frequency uses an exact unsigned ratio. Both terms must\n"
        << "  be non-zero before a reciprocal Period exists.\n"
        << "============================================================\n";
    return 0;
}
