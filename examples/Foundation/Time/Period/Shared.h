#ifndef FOUNDATION_EXAMPLES_TIME_PERIOD_SHARED_H
#define FOUNDATION_EXAMPLES_TIME_PERIOD_SHARED_H

#include <Foundation/Math/Ratio.h>
#include <Foundation/Time/Period.h>

namespace FoundationExamples {
    namespace Time {
        namespace Period {

            typedef void (*RatioPrinter)(
                const char* name,
                const Foundation::Math::Ratio& ratio
            );

            typedef void (*PeriodPrinter)(
                const char* name,
                const Foundation::Time::Period& period
            );

            void Run(
                RatioPrinter ratioPrinter,
                PeriodPrinter periodPrinter
            );

        }
    }
}

#endif
