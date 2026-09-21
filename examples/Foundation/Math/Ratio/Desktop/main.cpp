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

}

int main() {
    FoundationExamples::Math::Ratio::Run(PrintRatio);
    return 0;
}
