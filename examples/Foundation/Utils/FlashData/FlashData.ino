#include <Foundation.h>
#include "Shared.h"

void setup() {
    Serial.begin(115200);

    const FoundationExamples::Utils::FlashData::Result result =
        FoundationExamples::Utils::FlashData::Run();

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: Utils / FlashData");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Keep constant tables and strings in program memory.");
    Serial.println();
    Serial.println("[1] READ ONE TABLE ENTRY");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    Foundation::Utils::Flash::Read(&Table[1]);");
    Serial.print("  Value ................ ");
    Serial.println(result.SecondValue);
    Serial.println();
    Serial.println("[2] COPY A STRING INTO A BUFFER");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    Foundation::Utils::Flash::CopyString(buffer, 8, Name);");
    Serial.print("  Buffer of 8 .......... ");
    Serial.println(result.Name);
    Serial.print("  Returned length ...... ");
    Serial.println(result.NameLength);
    Serial.print("  Buffer of 4 .......... ");
    Serial.println(result.Truncated);
    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  FOUNDATION_FLASH places data; Flash::Read and CopyString");
    Serial.println("  read it back on every target.");
    Serial.println("============================================================");
}

void loop() {
}
