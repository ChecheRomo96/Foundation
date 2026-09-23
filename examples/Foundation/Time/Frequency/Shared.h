#ifndef FOUNDATION_EXAMPLES_TIME_FREQUENCY_SHARED_H
#define FOUNDATION_EXAMPLES_TIME_FREQUENCY_SHARED_H

#include <Foundation/Math/Ratio.h>
#include <Foundation/Time/Frequency.h>

namespace FoundationExamples {
    namespace Time {
        namespace Frequency {

            typedef void (*RatioPrinter)(
                const char* name,
                const char* code,
                const Foundation::Math::Ratio& ratio
            );

            typedef void (*FrequencyPrinter)(
                const char* name,
                const char* code,
                const Foundation::Time::Frequency& frequency
            );

            void Run(
                RatioPrinter ratioPrinter,
                FrequencyPrinter frequencyPrinter
            );

        }
    }
}

#endif
