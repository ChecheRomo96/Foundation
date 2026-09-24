#include "Shared.h"

namespace {

    void PrintRatio(
        const char* name,
        const char* code,
        const Foundation::Math::Ratio& ratio
    ) {
        Serial.println(name);
        Serial.println("------------------------------------------------------------");
        Serial.println("  CODE");
        Serial.print("    ");
        Serial.println(code);
        Serial.print("  Numerator ............ ");
        Serial.println(ratio.Numerator());
        Serial.print("  Denominator .......... ");
        Serial.println(ratio.Denominator());
        Serial.print("  Valid denominator .... ");
        Serial.println(ratio.IsValid() ? "yes" : "no");
        Serial.print("  Decimal view ......... ");
        Serial.println(ratio.ToFloat(), 6);
        Serial.println();
    }

}

void setup() {
    Serial.begin(115200);

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: Math / Ratio");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Preserve exact numerator/denominator values and convert");
    Serial.println("  them to floating point only when needed.");
    Serial.println();

    FoundationExamples::Math::Ratio::Run(PrintRatio);

    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  Ratio keeps values such as 30000/1001 exact instead of");
    Serial.println("  beginning with a rounded decimal.");
    Serial.println("============================================================");
}

void loop() {
}
