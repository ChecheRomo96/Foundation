#ifndef FOUNDATION_EXAMPLES_MATH_RATIO_SHARED_H
#define FOUNDATION_EXAMPLES_MATH_RATIO_SHARED_H

#include <Foundation/Math/Ratio.h>

namespace FoundationExamples {
    namespace Math {
        namespace Ratio {

            typedef void (*Printer)(
                const char* name,
                const Foundation::Math::Ratio& ratio
            );

            void Run(Printer printer);

        }
    }
}

#endif
