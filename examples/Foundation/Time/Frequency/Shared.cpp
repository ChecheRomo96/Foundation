#include "Shared.h"

#include <Foundation/Time/Period.h>

namespace FoundationExamples {
    namespace Time {
        namespace Frequency {

            void Run(
                RatioPrinter ratioPrinter,
                FrequencyPrinter frequencyPrinter
            ) {
                using Foundation::Math::Ratio;
                using Foundation::Time::Frequency;
                using Foundation::Time::Period;

                if (ratioPrinter == nullptr || frequencyPrinter == nullptr) {
                    return;
                }

                const Ratio audioRatio(48000, 1);
                ratioPrinter("Ratio audioRatio(48000, 1)", audioRatio);

                const Frequency samplingRate(48000, 1);
                frequencyPrinter("Frequency samplingRate(48000, 1)", samplingRate);

                const Period samplingPeriod = samplingRate.GetPeriod();
                const Frequency roundTripRate = samplingPeriod.GetFrequency();
                frequencyPrinter(
                    "Frequency roundTripRate = samplingPeriod.GetFrequency()",
                    roundTripRate
                );

                const Frequency videoRate(30000, 1001);
                frequencyPrinter("Frequency videoRate(30000, 1001)", videoRate);

                Frequency mutableFrequency;
                mutableFrequency.Set(1000, 1);
                frequencyPrinter(
                    "Frequency mutableFrequency after Set(1000, 1)",
                    mutableFrequency
                );

                mutableFrequency.SetNum(44100);
                mutableFrequency.SetDen(1);
                frequencyPrinter(
                    "Frequency mutableFrequency after SetNum(44100), SetDen(1)",
                    mutableFrequency
                );

                const Ratio rawFrequencyRatio(96000, 1);
                const Frequency frequencyFromRatio(rawFrequencyRatio);
                frequencyPrinter(
                    "Frequency frequencyFromRatio(Ratio(96000, 1))",
                    frequencyFromRatio
                );
                ratioPrinter(
                    "frequencyFromRatio.GetRatio()",
                    frequencyFromRatio.GetRatio()
                );

                const Frequency invalidFrequency(1, 0);
                frequencyPrinter(
                    "Frequency invalidFrequency(1, 0)",
                    invalidFrequency
                );
            }

        }
    }
}
