#include "Shared.h"

void setup() {
    Serial.begin(115200);

    const FoundationExamples::Containers::DataStructures::Result result =
        FoundationExamples::Containers::DataStructures::Run();

    Serial.println("============================================================");
    Serial.println(" FOUNDATION :: Containers / DataStructures");
    Serial.println("============================================================");
    Serial.println();
    Serial.println("PURPOSE");
    Serial.println("  Compare bounded FIFO and LIFO containers using caller-");
    Serial.println("  provided storage and no heap allocation.");
    Serial.println();
    Serial.println("[1] CIRCULAR BUFFER / WRAPAROUND");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    circularBuffer.Push(1);");
    Serial.println("    circularBuffer.Pop(first);");
    Serial.println("    circularBuffer.Push(4);");
    Serial.println("  Scenario ............ Push [1, 2, 3], pop 1, push 4, drain");
    Serial.print("  First removed ....... ");
    Serial.println(result.CircularBufferFirst);
    Serial.print("  Last removed ........ ");
    Serial.println(result.CircularBufferLast);
    Serial.print("  Empty after drain ... ");
    Serial.println(result.CircularBufferEmpty ? "yes" : "no");
    Serial.println();
    Serial.println("[2] QUEUE / FIFO ORDER");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    queue.Push(10); queue.Push(20); queue.Pop(front);");
    Serial.println("  Scenario ............ Push [10, 20], then pop once");
    Serial.print("  Front removed ....... ");
    Serial.println(result.QueueFront);
    Serial.println();
    Serial.println("[3] STACK / LIFO ORDER");
    Serial.println("------------------------------------------------------------");
    Serial.println("  CODE");
    Serial.println("    stack.Push(5); stack.Push(6); stack.Pop(top);");
    Serial.println("  Scenario ............ Push [5, 6], then pop once");
    Serial.print("  Top removed ......... ");
    Serial.println(result.StackTop);
    Serial.print("  Values remaining .... ");
    Serial.println(result.StackRemaining);
    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("TAKEAWAY");
    Serial.println("  Queue/CircularBuffer remove the oldest value; Stack");
    Serial.println("  removes the newest value.");
    Serial.println("============================================================");
}

void loop() {
}
