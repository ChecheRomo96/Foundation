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

    void PrintPeriod(
        const char* name,
        const Foundation::Time::Period& period
    ) {
        std::cout << name << '\n';
        std::cout << "  Num:           " << period.Num() << '\n';
        std::cout << "  Den:           " << period.Den() << '\n';
        std::cout << "  IsValid:       " << period.IsValid() << '\n';
        std::cout << "  Seconds:       " << period.Seconds() << " s\n";
        std::cout << "  Milliseconds:  "
                  << period.Milliseconds() << " ms\n";
        std::cout << "  Microseconds:  "
                  << period.Microseconds() << " us\n\n";
    }

}

int main() {
    FoundationExamples::Time::Period::Run(PrintRatio, PrintPeriod);
    return 0;
}
