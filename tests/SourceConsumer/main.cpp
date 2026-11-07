#include <cstdlib>
#include <iostream>

#include <Foundation.h>

int main() {
    const Foundation::Math::Ratio ratio(3, 4);

    if (!ratio.IsValid() || ratio.Numerator() != 3 || ratio.Denominator() != 4) {
        std::cerr << "Foundation source consumer: FAILED\n";
        return EXIT_FAILURE;
    }

    std::cout
        << "Foundation source consumer: PASSED\n"
        << "  target: Foundation::Foundation\n";
    return EXIT_SUCCESS;
}
