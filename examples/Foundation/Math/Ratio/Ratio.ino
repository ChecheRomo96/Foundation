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

}

void setup() {
    Serial.begin(115200);
    FoundationExamples::Math::Ratio::Run(PrintRatio);
}

void loop() {
}
