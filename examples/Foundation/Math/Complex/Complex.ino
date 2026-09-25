#include "Shared.h"

namespace {

    void PrintComplex(const char* name, const Foundation::Math::Complex<float>& value) {
        Serial.print(name);
        Serial.print(": ");
        Serial.print(value.real, 3);
        Serial.print(value.imag < 0.0f ? " - " : " + ");
        Serial.print(value.imag < 0.0f ? -value.imag : value.imag, 3);
        Serial.println("i");
    }

}

void setup() {
    Serial.begin(115200);

    const FoundationExamples::Math::Complex::Result result =
        FoundationExamples::Math::Complex::Run();

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: Math / Complex");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Demonstrate arithmetic and derived values for complex");
    Serial.println("  numbers.");
    Serial.println();
    Serial.println("[1] OPERANDS");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    Foundation::Math::Complex<float> a(3.0f, 4.0f);");
    Serial.println("    Foundation::Math::Complex<float> b(1.0f, -2.0f);");
    PrintComplex("  a ...................", result.First);
    PrintComplex("  b ...................", result.Second);
    Serial.println();
    Serial.println("[2] OPERATIONS");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    a + b;");
    Serial.println("    a * b;");
    Serial.println("    a.Conjugate();");
    Serial.println("    a.MagnitudeSquared();");
    PrintComplex("  a + b ...............", result.Sum);
    PrintComplex("  a * b ...............", result.Product);
    PrintComplex("  conjugate(a) ........", result.Conjugate);
    Serial.print("  magnitudeSquared(a) . ");
    Serial.println(result.MagnitudeSquared, 3);
    Serial.println("  Meaning ............. 3^2 + 4^2; no square root needed");
    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  Complex keeps real/imaginary arithmetic explicit and");
    Serial.println("  provides common derived operations.");
    Serial.println("============================================================");
}

void loop() {
}
