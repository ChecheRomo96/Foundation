#include "Shared.h"

void setup() {
    Serial.begin(115200);

    const FoundationExamples::Math::Arithmetic::Result result =
        FoundationExamples::Math::Arithmetic::Run();

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: Math / Arithmetic");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Demonstrate integer GCD and angle conversions.");
    Serial.println();
    Serial.println("[1] GREATEST COMMON DIVISOR");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    Foundation::Math::GCD(84, 30);");
    Serial.println("  Input ............... 84 and 30");
    Serial.print("  Result .............. ");
    Serial.println(result.GreatestCommonDivisor);
    Serial.println("  Meaning ............. Largest integer dividing both inputs");
    Serial.println();
    Serial.println("[2] DEGREES TO RADIANS");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    Foundation::Math::DegToRad(180.0f);");
    Serial.print("  Result .............. ");
    Serial.print(result.HalfTurnRadians, 6);
    Serial.println(" rad (approximately Pi)");
    Serial.println();
    Serial.println("[3] RADIANS TO DEGREES");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    Foundation::Math::RadToDeg(");
    Serial.println("        Foundation::Math::Pi / 2.0f);");
    Serial.print("  Result .............. ");
    Serial.print(result.QuarterTurnDegrees, 6);
    Serial.println(" degrees");
    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  Foundation provides the conversion and constant while the");
    Serial.println("  caller keeps the units explicit.");
    Serial.println("============================================================");
}

void loop() {
}
