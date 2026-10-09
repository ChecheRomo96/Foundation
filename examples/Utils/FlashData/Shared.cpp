#include "Shared.h"

namespace FoundationExamples {
namespace Utils {
namespace FlashData {

    namespace {

        // On AVR these stay in program memory; elsewhere they are ordinary
        // constants. Both are read only through Foundation::Utils::Flash.
        const uint16_t Table[] FOUNDATION_FLASH = {100, 250, 400};
        const char Name[] FOUNDATION_FLASH = "Foundation";

    } // namespace

    Result Run() {
        Result result;
        result.SecondValue = Foundation::Utils::Flash::Read(&Table[1]);
        result.NameLength = static_cast<uint8_t>(
            Foundation::Utils::Flash::CopyString(result.Name, sizeof(result.Name), Name));
        Foundation::Utils::Flash::CopyString(result.Truncated, sizeof(result.Truncated), Name);
        return result;
    }

} // namespace FlashData
} // namespace Utils
} // namespace FoundationExamples
