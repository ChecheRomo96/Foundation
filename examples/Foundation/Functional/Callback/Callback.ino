#include "Shared.h"

void setup() {
    Serial.begin(115200);

    const FoundationExamples::Functional::Callback::Result result =
        FoundationExamples::Functional::Callback::Run();

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: Functional / Callback");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Bind free and member functions without allocation.");
    Serial.println();
    Serial.println("[1] FREE FUNCTION BINDING");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    freeFunction.Bind(Add);");
    Serial.println("    freeFunction.Invoke(4, 5);");
    Serial.println("  Target .............. Add(first, second)");
    Serial.println("  Invocation .......... Add(4, 5)");
    Serial.print("  Returned ............ ");
    Serial.println(result.FreeFunctionValue);
    Serial.println();
    Serial.println("[2] MEMBER FUNCTION BINDING");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    memberFunction.Bind<Accumulator,");
    Serial.println("        &Accumulator::AddValue>(&accumulator);");
    Serial.println("    memberFunction.Invoke(5);");
    Serial.println("    memberFunction.Invoke(3);");
    Serial.println("  Target .............. Accumulator::AddValue(value)");
    Serial.println("  Invocations ......... AddValue(5), then AddValue(3)");
    Serial.print("  Accumulated total ... ");
    Serial.println(result.MemberFunctionValue);
    Serial.println();
    Serial.println("[3] EXPLICIT BINDING STATE");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    memberFunction.IsBound();");
    Serial.println("    memberFunction.Unbind();");
    Serial.print("  Before unbind() ..... ");
    Serial.println(result.BoundBeforeUnbind ? "bound" : "empty");
    Serial.print("  After unbind() ...... ");
    Serial.println(result.BoundAfterUnbind ? "bound" : "empty");
    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  Callback presents one invocation API for free and member");
    Serial.println("  functions while keeping ownership external.");
    Serial.println("============================================================");
}

void loop() {
}
