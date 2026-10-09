#include "Shared.h"

#include <Foundation/Time/Frequency.h>

namespace FoundationExamples {
    namespace Time {
        namespace Period {

            void Run(
                UnsignedRatioPrinter ratioPrinter,
                PeriodPrinter periodPrinter
            ) {
                using Foundation::Math::UnsignedRatio;
                using Foundation::Time::Frequency;
                using Foundation::Time::Period;

                if (ratioPrinter == nullptr || periodPrinter == nullptr) {
                    return;
                }

                const Period samplingPeriod(1, 48000);
                periodPrinter(
                    "[1] AUDIO SAMPLING PERIOD",
                    "const Period samplingPeriod(1, 48000);",
                    samplingPeriod
                );

                const Frequency samplingRate = samplingPeriod.GetFrequency();
                const Period roundTripPeriod = samplingRate.GetPeriod();
                periodPrinter(
                    "[2] ROUND TRIP / PERIOD -> FREQUENCY -> PERIOD",
                    "const Frequency samplingRate =\n"
                    "        samplingPeriod.GetFrequency();\n"
                    "    const Period roundTripPeriod = samplingRate.GetPeriod();",
                    roundTripPeriod
                );

                const Period videoPeriod(1001, 30000);
                periodPrinter(
                    "[3] FRACTIONAL NTSC VIDEO PERIOD",
                    "const Period videoPeriod(1001, 30000);",
                    videoPeriod
                );

                Period mutablePeriod;
                mutablePeriod.Set(1, 1000);
                periodPrinter(
                    "[4] MUTATION / SET(1, 1000)",
                    "mutablePeriod.Set(1, 1000);",
                    mutablePeriod
                );

                mutablePeriod.SetNumerator(1);
                mutablePeriod.SetDenominator(48000);
                periodPrinter(
                    "[5] MUTATION / SETNUM(1), SETDEN(48000)",
                    "mutablePeriod.SetNumerator(1);\n"
                    "    mutablePeriod.SetDenominator(48000);",
                    mutablePeriod
                );

                const UnsignedRatio rawPeriodRatio(1, 96000);
                const Period periodFromRatio(rawPeriodRatio);
                periodPrinter(
                    "[6] CONSTRUCTION FROM RATIO(1, 96000)",
                    "const Period periodFromRatio(rawPeriodRatio);",
                    periodFromRatio
                );
                ratioPrinter(
                    "[7] RATIO VIEW / PERIOD::GETRATIO()",
                    "periodFromRatio.GetRatio();",
                    periodFromRatio.GetRatio()
                );

                const Period invalidPeriod(0, 1);
                periodPrinter(
                    "[8] INVALID INPUT / ZERO PERIOD",
                    "const Period invalidPeriod(0, 1);",
                    invalidPeriod
                );
            }

        }
    }
}
