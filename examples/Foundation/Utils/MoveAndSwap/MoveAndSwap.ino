#include "Shared.h"

void setup() {
    Serial.begin(115200);

    const FoundationExamples::Utils::MoveAndSwap::Result result =
        FoundationExamples::Utils::MoveAndSwap::Run();

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: Utils / MoveAndSwap");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Demonstrate Move and Swap without <utility>.");
    Serial.println();
    Serial.println("[1] SWAP TWO INTEGERS");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    Foundation::Utils::Swap(first, second);");
    Serial.println("  Before ............... first=3, second=9");
    Serial.print("  First after .......... ");
    Serial.println(result.FirstAfterSwap);
    Serial.print("  Second after ......... ");
    Serial.println(result.SecondAfterSwap);
    Serial.println();
    Serial.println("[2] MOVE A NON-COPYABLE VALUE");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    MoveOnlyValue destination(");
    Serial.println("        Foundation::Utils::Move(source));");
    Serial.println("  Source before ........ 42");
    Serial.print("  Destination after .... ");
    Serial.println(result.MovedToValue);
    Serial.print("  Source after ......... ");
    Serial.println(result.MovedFromValue);
    Serial.println("  Move performs the cast; the type defines the result.");
    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  Move enables transfer semantics; Swap builds an exchange");
    Serial.println("  from move operations.");
    Serial.println("============================================================");
}

void loop() {
}
