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
        Serial.print("  Valid ................ ");
        Serial.println(ratio.IsValid() ? "yes" : "no");
        Serial.print("  Decimal view ......... ");
        Serial.println(ratio.ToFloat(), 6);
        Serial.println();
    }

    void PrintPeriod(
        const char* name,
        const char* code,
        const Foundation::Time::Period& period
    ) {
        Serial.println(name);
        Serial.println("------------------------------------------------------------");
        Serial.println("  CODE");
        Serial.print("    ");
        Serial.println(code);
        Serial.print("  Numerator ............ ");
        Serial.println(period.Numerator());
        Serial.print("  Denominator .......... ");
        Serial.println(period.Denominator());
        Serial.print("  Valid ................ ");
        Serial.println(period.IsValid() ? "yes" : "no");
        Serial.print("  Seconds/tick ......... ");
        Serial.print(period.Seconds(), 6);
        Serial.println(" s");
        Serial.print("  Milliseconds/tick .... ");
        Serial.print(period.Milliseconds(), 6);
        Serial.println(" ms");
        Serial.print("  Microseconds/tick .... ");
        Serial.print(period.Microseconds(), 6);
        Serial.println(" us");
        Serial.println();
    }

}

void setup() {
    Serial.begin(115200);

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: Time / Period");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Represent seconds-per-tick exactly, derive rates, mutate");
    Serial.println("  values, and expose invalid input.");
    Serial.println();

    FoundationExamples::Time::Period::Run(PrintRatio, PrintPeriod);

    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  Period stores exact seconds per tick; Frequency is its");
    Serial.println("  reciprocal.");
    Serial.println("============================================================");
}

void loop() {
}
