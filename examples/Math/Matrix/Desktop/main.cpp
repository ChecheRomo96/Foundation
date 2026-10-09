#include <iomanip>
#include <iostream>

#include "Shared.h"

namespace {

    unsigned int IntegerWidth(int value) {
        unsigned int width = value < 0 ? 2u : 1u;
        long magnitude = value;
        if (magnitude < 0) {
            magnitude = -magnitude;
        }
        while (magnitude >= 10) {
            magnitude /= 10;
            ++width;
        }
        return width;
    }

    template <unsigned int Rows, unsigned int Columns>
    void PrintMatrix(const char* name, const int (&values)[Rows][Columns]) {
        unsigned int columnWidth = 1;
        for (unsigned int row = 0; row < Rows; ++row) {
            for (unsigned int col = 0; col < Columns; ++col) {
                const unsigned int width = IntegerWidth(values[row][col]);
                if (width > columnWidth) {
                    columnWidth = width;
                }
            }
        }

        std::cout << "  " << name << '\n';
        for (unsigned int row = 0; row < Rows; ++row) {
            const char* leftBracket = row == 0
                ? u8"⌈"
                : (row + 1 == Rows ? u8"⌊" : "|");
            const char* rightBracket = row == 0
                ? u8"⌉"
                : (row + 1 == Rows ? u8"⌋" : "|");
            std::cout << "  " << leftBracket;
            for (unsigned int col = 0; col < Columns; ++col) {
                std::cout << std::setw(columnWidth + 1) << values[row][col];
            }
            std::cout << ' ' << rightBracket << '\n';
        }
    }

}

int main() {
    const FoundationExamples::Math::Matrix::Result result =
        FoundationExamples::Math::Matrix::Run();

    std::cout
        << "============================================================\n"
        << " FOUNDATION :: Math / Matrix\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Compare 2x2 operations with fixed and dynamic storage.\n\n"
        << "[1] INPUT MATRICES\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    Matrix::Fixed<int, 2, 2> first(firstValues);\n"
        << "    Matrix::Fixed<int, 2, 2> second(secondValues);\n";
    PrintMatrix("A", result.First);
    PrintMatrix("B", result.Second);
    std::cout
        << '\n'
        << "[2] FIXED MATRIX / C = A * B\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    Foundation::Math::Matrix::Multiply(first, second, fixedProduct);\n";
    PrintMatrix("C", result.FixedProduct);
    std::cout
        << "  C[0][0] ............. 1*5 + 2*7 = "
        << result.FixedProduct[0][0] << '\n'
        << "  C[1][1] ............. 3*6 + 4*8 = "
        << result.FixedProduct[1][1] << "\n\n"
        << "[3] FIXED MATRIX / TRANSPOSE(A)\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    const auto fixedTranspose = first.Transposed();\n";
    PrintMatrix("transpose(A)", result.FixedTranspose);
    std::cout
        << "  [0][1] .............. copied from A[1][0] = "
        << result.FixedTranspose[0][1] << "\n\n"
        << "[4] DYNAMIC MATRIX / C = A * B\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    Foundation::Math::Matrix::Multiply(\n"
        << "        dynamicFirst, dynamicSecond, dynamicProduct);\n"
        << "  Operation succeeded . "
        << (result.DynamicMultiplySucceeded ? "yes" : "no") << '\n'
        << "  Result owns storage .. "
        << (result.DynamicResultOwnsData ? "yes" : "no") << '\n';
    PrintMatrix("dynamic C", result.DynamicProduct);
    std::cout
        << '\n'
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  Matrix::Fixed encodes dimensions in the type; Matrix::Dynamic\n"
        << "  checks dimensions at runtime.\n"
        << "============================================================\n";

    return result.FixedProduct[0][0] == 19
        && result.FixedProduct[0][1] == 22
        && result.FixedProduct[1][0] == 43
        && result.FixedProduct[1][1] == 50
        && result.FixedTranspose[0][1] == 3
        && result.DynamicMultiplySucceeded
        && result.DynamicResultOwnsData
        && result.DynamicProduct[0][0] == 19
        && result.DynamicProduct[0][1] == 22
        && result.DynamicProduct[1][0] == 43
        && result.DynamicProduct[1][1] == 50
        ? 0
        : 1;
}
