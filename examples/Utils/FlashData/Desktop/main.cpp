#include <cstring>
#include <iostream>

#include "Shared.h"

int main() {
    const FoundationExamples::Utils::FlashData::Result result =
        FoundationExamples::Utils::FlashData::Run();

    std::cout
        << "============================================================\n"
        << " FOUNDATION :: Utils / FlashData\n"
        << "============================================================\n"
        << "\nPURPOSE\n"
        << "  Keep constant tables and strings in program memory on AVR\n"
        << "  and read them the same way on every target.\n\n"
        << "[1] READ ONE TABLE ENTRY\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    const uint16_t Table[] FOUNDATION_FLASH = {100, 250, 400};\n"
        << "    Foundation::Utils::Flash::Read(&Table[1]);\n"
        << "  Value ................ " << result.SecondValue << "\n\n"
        << "[2] COPY A STRING INTO A BUFFER\n"
        << "------------------------------------------------------------\n"
        << "  CODE\n"
        << "    Foundation::Utils::Flash::CopyString(buffer, 8, Name);\n"
        << "  Buffer of 8 .......... " << result.Name << '\n'
        << "  Returned length ...... " << static_cast<int>(result.NameLength) << '\n'
        << "  Buffer of 4 .......... " << result.Truncated << '\n'
        << "  Note: like snprintf, the full length is returned so that\n"
        << "  truncation can be detected.\n\n"
        << "------------------------------------------------------------\n"
        << "TAKEAWAY\n"
        << "  FOUNDATION_FLASH places data; Flash::Read and CopyString\n"
        << "  are the only supported way to read it back.\n"
        << "============================================================\n";

    return result.SecondValue == 250
        && std::strcmp(result.Name, "Foundat") == 0
        && result.NameLength == 10
        && std::strcmp(result.Truncated, "Fou") == 0
        ? 0
        : 1;
}
