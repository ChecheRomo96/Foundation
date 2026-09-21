#include "Shared.h"

void setup() {
    Serial.begin(115200);

    const FoundationExamples::TypeTraits::BasicChecks::Result result =
        FoundationExamples::TypeTraits::BasicChecks::Run();

    Serial.println("Foundation TypeTraits BasicChecks example");
    Serial.print("Buffer[0]: ");
    Serial.println(result.BufferFirst);
    Serial.print("Buffer size: ");
    Serial.println(result.BufferSize);
    Serial.print("Sample channel: ");
    Serial.println(result.Sample.Channel);
    Serial.print("Sample value: ");
    Serial.println(result.Sample.Value, 6);
}

void loop() {
}
