#include "Shared.h"

void setup() {
    Serial.begin(115200);

    const FoundationExamples::TypeTraits::BasicChecks::Result result =
        FoundationExamples::TypeTraits::BasicChecks::Run();

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: TypeTraits / BasicChecks");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Classify types and constrain a generic NumericBuffer<T>.");
    Serial.println();
    Serial.println("[1] COMPILE-TIME CLASSIFICATION");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    static_assert(is_integral<int>::value, \"...\");");
    Serial.println("  static_assert verified the supported type categories.");
    Serial.println("  Runtime cost ......... none");
    Serial.println("  Failure behavior ..... compilation stops");
    Serial.println();
    Serial.println("[2] CONSTRAINED NUMERIC BUFFER");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    NumericBuffer<int> buffer(data, 4);");
    Serial.println("    buffer.Set(0, 10);");
    Serial.println("  Initial values ....... [1, 2, 3, 4]");
    Serial.println("  Constraint ........... is_arithmetic<int>");
    Serial.print("  Set(0, 10) ........... first value = ");
    Serial.println(result.BufferFirst);
    Serial.print("  Buffer size .......... ");
    Serial.println(result.BufferSize);
    Serial.println();
    Serial.println("[3] USER-DEFINED OBJECT");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    ProcessObject<SensorSample>(sample);");
    Serial.println("  Constraint ........... is_object<SensorSample>");
    Serial.print("  Channel .............. ");
    Serial.println(result.Sample.Channel);
    Serial.print("  Value ................ ");
    Serial.println(result.Sample.Value, 6);
    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  TypeTraits moves invalid generic combinations from runtime");
    Serial.println("  behavior to compile-time diagnostics.");
    Serial.println("============================================================");
}

void loop() {
}
