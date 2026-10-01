#ifndef FOUNDATION_EXAMPLES_UTILS_FLASH_DATA_SHARED_H
#define FOUNDATION_EXAMPLES_UTILS_FLASH_DATA_SHARED_H

#include <stdint.h>

#include <Foundation/Utils.h>

namespace FoundationExamples {
namespace Utils {
namespace FlashData {

    struct Result {
        uint16_t SecondValue;
        char Name[8];
        uint8_t NameLength;
        char Truncated[4];
    };

    Result Run();

} // namespace FlashData
} // namespace Utils
} // namespace FoundationExamples

#endif
