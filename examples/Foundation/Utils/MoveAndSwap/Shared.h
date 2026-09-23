#ifndef FOUNDATION_EXAMPLES_UTILS_MOVE_AND_SWAP_SHARED_H
#define FOUNDATION_EXAMPLES_UTILS_MOVE_AND_SWAP_SHARED_H

#include <Foundation/Utils.h>

namespace FoundationExamples {
namespace Utils {
namespace MoveAndSwap {

    struct Result {
        int FirstAfterSwap;
        int SecondAfterSwap;
        int MovedFromValue;
        int MovedToValue;
    };

    Result Run();

} // namespace MoveAndSwap
} // namespace Utils
} // namespace FoundationExamples

#endif
