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

    void PrintFrequency(
        const char* name,
        const Foundation::Time::Frequency& frequency
    ) {
        Serial.println(name);
        Serial.print("  Num:                 ");
        Serial.println(frequency.Num());
        Serial.print("  Den:                 ");
        Serial.println(frequency.Den());
        Serial.print("  IsValid:             ");
        Serial.println(frequency.IsValid() ? "true" : "false");
        Serial.print("  Hertz:               ");
        Serial.println(frequency.Hertz(), 6);
        Serial.print("  PeriodSeconds:       ");
        Serial.println(frequency.PeriodSeconds(), 6);
        Serial.print("  PeriodMilliseconds:  ");
        Serial.println(frequency.PeriodMilliseconds(), 6);
        Serial.print("  PeriodMicroseconds:  ");
        Serial.println(frequency.PeriodMicroseconds(), 6);
        Serial.println();
    }

}

void setup() {
    Serial.begin(115200);
    FoundationExamples::Time::Frequency::Run(PrintRatio, PrintFrequency);
}

void loop() {
}
