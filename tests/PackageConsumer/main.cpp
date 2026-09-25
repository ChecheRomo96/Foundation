#include <cstdlib>
#include <iostream>

#include <Foundation.h>

bool ValidateSelectiveHeaders();

int main() {
    const Foundation::Math::Ratio ratio(3, 4);

    const bool umbrellaIsUsable =
        ratio.IsValid() &&
        ratio.Numerator() == 3 &&
        ratio.Denominator() == 4;

    if (!umbrellaIsUsable || !ValidateSelectiveHeaders()) {
        std::cerr << "Foundation package consumer: FAILED\n";
        return EXIT_FAILURE;
    }

    std::cout
        << "Foundation package consumer: PASSED\n"
        << "  umbrella header: Foundation.h\n"
        << "  selective headers: Frequency.h, Period.h\n"
        << "  imported target: Foundation::Foundation\n"
        << "  linked symbol: Frequency::GetPeriod()\n";

    return EXIT_SUCCESS;
}
