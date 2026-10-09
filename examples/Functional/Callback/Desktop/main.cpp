#include <iostream>

#include "Shared.h"

int main() {
    const FoundationExamples::Functional::Callback::Result result =
        FoundationExamples::Functional::Callback::Run();

    std::cout
        << "============================================================\n"
        << " FOUNDATION :: Functional / Callback\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Bind free and member functions without allocation.\n\n"
        << "[1] FREE FUNCTION BINDING\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    freeFunction.Bind(Add);\n"
        << "    freeFunction.Invoke(4, 5);\n"
        << "  Target .............. Add(first, second)\n"
        << "  Invocation .......... Add(4, 5)\n"
        << "  Returned ............ " << result.FreeFunctionValue << "\n\n"
        << "[2] MEMBER FUNCTION BINDING\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    memberFunction.Bind<Accumulator,\n"
        << "        &Accumulator::AddValue>(&accumulator);\n"
        << "    memberFunction.Invoke(5);\n"
        << "    memberFunction.Invoke(3);\n"
        << "  Target .............. Accumulator::AddValue(value)\n"
        << "  Invocations ......... AddValue(5), then AddValue(3)\n"
        << "  Accumulated total ... " << result.MemberFunctionValue << "\n\n"
        << "[3] EXPLICIT BINDING STATE\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    memberFunction.IsBound();\n"
        << "    memberFunction.Unbind();\n"
        << "  Before unbind() ..... "
        << (result.BoundBeforeUnbind ? "bound" : "empty") << '\n'
        << "  After unbind() ...... "
        << (result.BoundAfterUnbind ? "bound" : "empty") << "\n\n"
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  Callback presents one invocation API for free and member\n"
        << "  functions while keeping ownership external.\n"
        << "============================================================\n";

    return result.FreeFunctionValue == 9
        && result.MemberFunctionValue == 8
        && result.BoundBeforeUnbind
        && !result.BoundAfterUnbind
        ? 0
        : 1;
}
