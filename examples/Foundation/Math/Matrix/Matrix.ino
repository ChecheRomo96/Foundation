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

    void PrintCell(int value, unsigned int columnWidth) {
        const unsigned int width = IntegerWidth(value);
        for (unsigned int padding = width; padding <= columnWidth; ++padding) {
            Serial.print(' ');
        }
        Serial.print(value);
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

        Serial.print("  ");
        Serial.println(name);
        for (unsigned int row = 0; row < Rows; ++row) {
            const char* leftBracket = row == 0
                ? u8"⌈"
                : (row + 1 == Rows ? u8"⌊" : "|");
            const char* rightBracket = row == 0
                ? u8"⌉"
                : (row + 1 == Rows ? u8"⌋" : "|");
            Serial.print("  ");
            Serial.print(leftBracket);
            for (unsigned int col = 0; col < Columns; ++col) {
                PrintCell(values[row][col], columnWidth);
            }
            Serial.print(' ');
            Serial.println(rightBracket);
        }
    }

}

void setup() {
    Serial.begin(115200);

    const FoundationExamples::Math::Matrix::Result result =
        FoundationExamples::Math::Matrix::Run();

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: Math / Matrix");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Compare 2x2 operations with fixed and dynamic storage.");
    Serial.println();
    Serial.println("[1] INPUT MATRICES");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    Matrix::Fixed<int, 2, 2> first(firstValues);");
    Serial.println("    Matrix::Fixed<int, 2, 2> second(secondValues);");
    PrintMatrix("A", result.First);
    PrintMatrix("B", result.Second);
    Serial.println();
    Serial.println("[2] FIXED MATRIX / C = A * B");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    Foundation::Math::Matrix::Multiply(first, second, fixedProduct);");
    PrintMatrix("C", result.FixedProduct);
    Serial.print("  C[0][0] ............. 1*5 + 2*7 = ");
    Serial.println(result.FixedProduct[0][0]);
    Serial.print("  C[1][1] ............. 3*6 + 4*8 = ");
    Serial.println(result.FixedProduct[1][1]);
    Serial.println();
    Serial.println("[3] FIXED MATRIX / TRANSPOSE(A)");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    const auto fixedTranspose = first.Transposed();");
    PrintMatrix("transpose(A)", result.FixedTranspose);
    Serial.print("  [0][1] .............. copied from A[1][0] = ");
    Serial.println(result.FixedTranspose[0][1]);
    Serial.println();
    Serial.println("[4] DYNAMIC MATRIX / C = A * B");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    Foundation::Math::Matrix::Multiply(");
    Serial.println("        dynamicFirst, dynamicSecond, dynamicProduct);");
    Serial.print("  Operation succeeded . ");
    Serial.println(result.DynamicMultiplySucceeded ? "yes" : "no");
    Serial.print("  Result owns storage .. ");
    Serial.println(result.DynamicResultOwnsData ? "yes" : "no");
    PrintMatrix("dynamic C", result.DynamicProduct);
    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  Matrix::Fixed encodes dimensions in the type; Matrix::Dynamic");
    Serial.println("  checks dimensions at runtime.");
    Serial.println("============================================================");
}

void loop() {
}
