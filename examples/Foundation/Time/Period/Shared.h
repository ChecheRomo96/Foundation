#ifndef FOUNDATION_EXAMPLES_TIME_PERIOD_SHARED_H
#define FOUNDATION_EXAMPLES_TIME_PERIOD_SHARED_H

#include <Foundation/Math/Ratio.h>
#include <Foundation/Time/Period.h>

namespace FoundationExamples {
    namespace Time {
        namespace Period {

            typedef void (*UnsignedRatioPrinter)(
                const char* name,
                const char* code,
                const Foundation::Math::UnsignedRatio& ratio
            );

            typedef void (*PeriodPrinter)(
                const char* name,
                const char* code,
                const Foundation::Time::Period& period
            );

            void Run(
                UnsignedRatioPrinter ratioPrinter,
                PeriodPrinter periodPrinter
            );

        }
    }
}

#endif
