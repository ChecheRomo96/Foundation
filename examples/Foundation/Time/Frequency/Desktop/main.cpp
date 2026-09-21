#include <iostream>

#include "Shared.h"

namespace {

    void PrintRatio(
        const char* name,
        const Foundation::Math::Ratio& ratio
    ) {
        std::cout << name << '\n';
        std::cout << "  Num:      " << ratio.Num() << '\n';
        std::cout << "  Den:      " << ratio.Den() << '\n';
        std::cout << "  IsValid:  " << ratio.IsValid() << '\n';
        std::cout << "  ToFloat:  " << ratio.ToFloat() << "\n\n";
    }

    void PrintFrequency(
        const char* name,
        const Foundation::Time::Frequency& frequency
    ) {
        std::cout << name << '\n';
        std::cout << "  Num:                 " << frequency.Num() << '\n';
        std::cout << "  Den:                 " << frequency.Den() << '\n';
        std::cout << "  IsValid:             " << frequency.IsValid() << '\n';
        std::cout << "  Hertz:               " << frequency.Hertz() << " Hz\n";
        std::cout << "  PeriodSeconds:       "
                  << frequency.PeriodSeconds() << " s\n";
        std::cout << "  PeriodMilliseconds:  "
                  << frequency.PeriodMilliseconds() << " ms\n";
        std::cout << "  PeriodMicroseconds:  "
                  << frequency.PeriodMicroseconds() << " us\n\n";
    }

}

int main() {
    FoundationExamples::Time::Frequency::Run(PrintRatio, PrintFrequency);
    return 0;
}
