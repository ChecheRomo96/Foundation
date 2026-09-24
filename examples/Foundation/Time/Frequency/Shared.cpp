#include "Shared.h"

#include <Foundation/Time/Period.h>

namespace FoundationExamples {
    namespace Time {
        namespace Frequency {

            void Run(
                UnsignedRatioPrinter ratioPrinter,
                FrequencyPrinter frequencyPrinter
            ) {
                using Foundation::Math::UnsignedRatio;
                using Foundation::Time::Frequency;
                using Foundation::Time::Period;

                if (ratioPrinter == nullptr || frequencyPrinter == nullptr) {
                    return;
                }

                const UnsignedRatio audioRatio(48000, 1);
                ratioPrinter(
                    "[1] SOURCE RATIO / AUDIO SAMPLING RATE",
                    "const UnsignedRatio audioRatio(48000, 1);",
                    audioRatio
                );

                const Frequency samplingRate(48000, 1);
                frequencyPrinter(
                    "[2] AUDIO SAMPLING FREQUENCY",
                    "const Frequency samplingRate(48000, 1);",
                    samplingRate
                );

                const Period samplingPeriod = samplingRate.GetPeriod();
                const Frequency roundTripRate = samplingPeriod.GetFrequency();
                frequencyPrinter(
                    "[3] ROUND TRIP / FREQUENCY -> PERIOD -> FREQUENCY",
                    "const Period samplingPeriod = samplingRate.GetPeriod();\n"
                    "    const Frequency roundTripRate = samplingPeriod.GetFrequency();",
                    roundTripRate
                );

                const Frequency videoRate(30000, 1001);
                frequencyPrinter(
                    "[4] FRACTIONAL NTSC VIDEO FREQUENCY",
                    "const Frequency videoRate(30000, 1001);",
                    videoRate
                );

                Frequency mutableFrequency;
                mutableFrequency.Set(1000, 1);
                frequencyPrinter(
                    "[5] MUTATION / SET(1000, 1)",
                    "mutableFrequency.Set(1000, 1);",
                    mutableFrequency
                );

                mutableFrequency.SetNum(44100);
                mutableFrequency.SetDen(1);
                frequencyPrinter(
                    "[6] MUTATION / SETNUM(44100), SETDEN(1)",
                    "mutableFrequency.SetNum(44100);\n"
                    "    mutableFrequency.SetDen(1);",
                    mutableFrequency
                );

                const UnsignedRatio rawFrequencyRatio(96000, 1);
                const Frequency frequencyFromRatio(rawFrequencyRatio);
                frequencyPrinter(
                    "[7] CONSTRUCTION FROM RATIO(96000, 1)",
                    "const Frequency frequencyFromRatio(rawFrequencyRatio);",
                    frequencyFromRatio
                );
                ratioPrinter(
                    "[8] RATIO VIEW / FREQUENCY::GETRATIO()",
                    "frequencyFromRatio.GetRatio();",
                    frequencyFromRatio.GetRatio()
                );

                const Frequency invalidFrequency(0, 1);
                frequencyPrinter(
                    "[9] INVALID INPUT / ZERO FREQUENCY",
                    "const Frequency invalidFrequency(0, 1);",
                    invalidFrequency
                );
            }

        }
    }
}
