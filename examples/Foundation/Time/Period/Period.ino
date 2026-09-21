#include "Shared.h"

namespace {

    void PrintRatio(
        const char* name,
        const Foundation::Math::Ratio& ratio
    ) {
        Serial.println(name);
        Serial.print("  Num:      ");
        Serial.println(ratio.Num());
        Serial.print("  Den:      ");
        Serial.println(ratio.Den());
        Serial.print("  IsValid:  ");
        Serial.println(ratio.IsValid() ? "true" : "false");
        Serial.print("  ToFloat:  ");
        Serial.println(ratio.ToFloat(), 6);
        Serial.println();
    }

    void PrintPeriod(
        const char* name,
        const Foundation::Time::Period& period
    ) {
        Serial.println(name);
        Serial.print("  Num:           ");
        Serial.println(period.Num());
        Serial.print("  Den:           ");
        Serial.println(period.Den());
        Serial.print("  IsValid:       ");
        Serial.println(period.IsValid() ? "true" : "false");
        Serial.print("  Seconds:       ");
        Serial.println(period.Seconds(), 6);
        Serial.print("  Milliseconds:  ");
        Serial.println(period.Milliseconds(), 6);
        Serial.print("  Microseconds:  ");
        Serial.println(period.Microseconds(), 6);
        Serial.println();
    }

}

void setup() {
    Serial.begin(115200);
    FoundationExamples::Time::Period::Run(PrintRatio, PrintPeriod);
}

void loop() {
}
