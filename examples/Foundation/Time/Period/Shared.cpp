#include "Shared.h"

#include <Foundation/Time/Frequency.h>

namespace FoundationExamples {
    namespace Time {
        namespace Period {

            void Run(
                RatioPrinter ratioPrinter,
                PeriodPrinter periodPrinter
            ) {
                using Foundation::Math::Ratio;
                using Foundation::Time::Frequency;
                using Foundation::Time::Period;

                if (ratioPrinter == nullptr || periodPrinter == nullptr) {
                    return;
                }

                const Period samplingPeriod(1, 48000);
                periodPrinter("Period samplingPeriod(1, 48000)", samplingPeriod);

                const Frequency samplingRate = samplingPeriod.GetFrequency();
                const Period roundTripPeriod = samplingRate.GetPeriod();
                periodPrinter(
                    "Period roundTripPeriod = samplingRate.GetPeriod()",
                    roundTripPeriod
                );

                const Period videoPeriod(1001, 30000);
                periodPrinter("Period videoPeriod(1001, 30000)", videoPeriod);

                Period mutablePeriod;
                mutablePeriod.Set(1, 1000);
                periodPrinter(
                    "Period mutablePeriod after Set(1, 1000)",
                    mutablePeriod
                );

                mutablePeriod.SetNum(1);
                mutablePeriod.SetDen(48000);
                periodPrinter(
                    "Period mutablePeriod after SetNum(1), SetDen(48000)",
                    mutablePeriod
                );

                const Ratio rawPeriodRatio(1, 96000);
                const Period periodFromRatio(rawPeriodRatio);
                periodPrinter(
                    "Period periodFromRatio(Ratio(1, 96000))",
                    periodFromRatio
                );
                ratioPrinter(
                    "periodFromRatio.GetRatio()",
                    periodFromRatio.GetRatio()
                );

                const Period invalidPeriod(1, 0);
                periodPrinter("Period invalidPeriod(1, 0)", invalidPeriod);
            }

        }
    }
}
