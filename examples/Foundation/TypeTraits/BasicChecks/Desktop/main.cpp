#include <iostream>

#include "Shared.h"

int main() {
    const FoundationExamples::TypeTraits::BasicChecks::Result result =
        FoundationExamples::TypeTraits::BasicChecks::Run();

    std::cout
        << "============================================================\n"
        << " FOUNDATION :: TypeTraits / BasicChecks\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Use compile-time traits to classify types and constrain a\n"
        << "  generic NumericBuffer<T>.\n\n"
        << "[1] COMPILE-TIME CLASSIFICATION\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    static_assert(is_integral<int>::value, \"...\");\n"
        << "  static_assert verified integral, floating-point, class,\n"
        << "  union, enum, pointer, reference, scalar, object,\n"
        << "  fundamental, compound, and function categories.\n"
        << "  Runtime cost ......... none\n"
        << "  Failure behavior ..... compilation stops\n\n"
        << "[2] CONSTRAINED NUMERIC BUFFER\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    NumericBuffer<int> buffer(data, 4);\n"
        << "    buffer.Set(0, 10);\n"
        << "  Initial values ....... [1, 2, 3, 4]\n"
        << "  Constraint ........... is_arithmetic<int>\n"
        << "  Set(0, 10) ........... first value = "
        << result.BufferFirst << ".\n"
        << "  Buffer size .......... " << result.BufferSize << "\n\n"
        << "[3] USER-DEFINED OBJECT\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    ProcessObject<SensorSample>(sample);\n"
        << "  Constraint ........... is_object<SensorSample>\n"
        << "  Sample ............... channel=" << result.Sample.Channel
        << ", value=" << result.Sample.Value << "\n\n"
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  TypeTraits moves invalid generic combinations from runtime\n"
        << "  behavior to compile-time diagnostics.\n"
        << "============================================================\n";

    return result.Score == 16 ? 0 : 1;
}
