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

    void PrintFrequency(
        const char* name,
        const char* code,
        const Foundation::Time::Frequency& frequency
    ) {
        Serial.println(name);
        Serial.println("------------------------------------------------------------");
        Serial.println("  CODE");
        Serial.print("    ");
        Serial.println(code);
        Serial.print("  Numerator ............ ");
        Serial.println(frequency.Numerator());
        Serial.print("  Denominator .......... ");
        Serial.println(frequency.Denominator());
        Serial.print("  Valid ................ ");
        Serial.println(frequency.IsValid() ? "yes" : "no");
        Serial.print("  Rate ................. ");
        Serial.print(frequency.Hertz(), 6);
        Serial.println(" Hz");
        Serial.print("  Seconds/tick ......... ");
        Serial.print(frequency.PeriodSeconds(), 6);
        Serial.println(" s");
        Serial.print("  Milliseconds/tick .... ");
        Serial.print(frequency.PeriodMilliseconds(), 6);
        Serial.println(" ms");
        Serial.print("  Microseconds/tick .... ");
        Serial.print(frequency.PeriodMicroseconds(), 6);
        Serial.println(" us");
        Serial.println();
    }

}

void setup() {
    Serial.begin(115200);

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: Time / Frequency");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Represent ticks-per-second exactly, derive time per tick,");
    Serial.println("  mutate values, and expose invalid input.");
    Serial.println();

    FoundationExamples::Time::Frequency::Run(PrintRatio, PrintFrequency);

    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  Frequency stores an exact ratio; Hertz is its value and");
    Serial.println("  Period is its reciprocal.");
    Serial.println("============================================================");
}

void loop() {
}
