#ifndef FOUNDATION_EXAMPLES_FUNCTIONAL_CALLBACK_SHARED_H
#define FOUNDATION_EXAMPLES_FUNCTIONAL_CALLBACK_SHARED_H

#include <Foundation/Functional/Callback.h>

namespace FoundationExamples {
namespace Functional {
namespace Callback {

    struct Result {
        int FreeFunctionValue;
        int MemberFunctionValue;
        bool BoundBeforeUnbind;
        bool BoundAfterUnbind;
    };

    Result Run();

} // namespace Callback
} // namespace Functional
} // namespace FoundationExamples

#endif
