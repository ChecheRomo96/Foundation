#include <iostream>

#include "Shared.h"

int main() {
    const FoundationExamples::Containers::DataStructures::Result result =
        FoundationExamples::Containers::DataStructures::Run();

    std::cout
        << "============================================================\n"
        << " FOUNDATION :: Containers / DataStructures\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Compare bounded FIFO and LIFO containers using caller-\n"
        << "  provided storage and no heap allocation.\n\n"
        << "[1] CIRCULAR BUFFER / WRAPAROUND\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    circularBuffer.Push(1);\n"
        << "    circularBuffer.Pop(first);\n"
        << "    circularBuffer.Push(4);\n"
        << "  Scenario ............ Push [1, 2, 3], pop 1, push 4, drain\n"
        << "  First removed ....... " << result.CircularBufferFirst << '\n'
        << "  Last removed ........ " << result.CircularBufferLast << '\n'
        << "  Empty after drain ... "
        << (result.CircularBufferEmpty ? "yes" : "no") << "\n\n"
        << "[2] QUEUE / FIFO ORDER\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    queue.Push(10); queue.Push(20); queue.Pop(front);\n"
        << "  Scenario ............ Push [10, 20], then pop once\n"
        << "  Front removed ....... " << result.QueueFront << "\n\n"
        << "[3] STACK / LIFO ORDER\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    stack.Push(5); stack.Push(6); stack.Pop(top);\n"
        << "  Scenario ............ Push [5, 6], then pop once\n"
        << "  Top removed ......... " << result.StackTop << '\n'
        << "  Values remaining .... " << result.StackRemaining << "\n\n"
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  Queue/CircularBuffer remove the oldest value; Stack\n"
        << "  removes the newest value.\n"
        << "============================================================\n";

    return result.CircularBufferFirst == 1
        && result.CircularBufferLast == 4
        && result.CircularBufferEmpty
        && result.QueueFront == 10
        && result.StackTop == 6
        && result.StackRemaining == 1
        ? 0
        : 1;
}
