#include <iostream>

#include "Shared.h"

int main() {
    const FoundationExamples::Utils::MoveAndSwap::Result result =
        FoundationExamples::Utils::MoveAndSwap::Run();

    std::cout
        << "============================================================\n"
        << " FOUNDATION :: Utils / MoveAndSwap\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Demonstrate the low-level operations used by Foundation\n"
        << "  containers without <utility>.\n\n"
        << "[1] SWAP TWO INTEGERS\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    Foundation::Utils::Swap(first, second);\n"
        << "  Before ............... first=3, second=9\n"
        << "  After ................ first=" << result.FirstAfterSwap
        << ", second=" << result.SecondAfterSwap << "\n\n"
        << "[2] MOVE A NON-COPYABLE VALUE\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    MoveOnlyValue destination(\n"
        << "        Foundation::Utils::Move(source));\n"
        << "  Source before ........ 42\n"
        << "  Destination after .... " << result.MovedToValue << '\n'
        << "  Source after ......... " << result.MovedFromValue << '\n'
        << "  Note: Move performs the cast; MoveOnlyValue's move\n"
        << "  constructor defines the source's resulting state.\n\n"
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  Move enables transfer semantics; Swap builds an exchange\n"
        << "  from move construction and assignment.\n"
        << "============================================================\n";

    return result.FirstAfterSwap == 9
        && result.SecondAfterSwap == 3
        && result.MovedFromValue == 0
        && result.MovedToValue == 42
        ? 0
        : 1;
}
