#ifndef FOUNDATION_EXAMPLES_TIME_CLOCK_AND_DURATION_SHARED_H
#define FOUNDATION_EXAMPLES_TIME_CLOCK_AND_DURATION_SHARED_H

#include <Foundation/Time.h>

namespace FoundationExamples {
namespace Time {
namespace ClockAndDuration {

    struct Result {
        Foundation::Time::Tick32::Representation StartingTick;
        Foundation::Time::Tick32::Representation EndingTick;
        Foundation::Time::Tick32::Representation ElapsedTicks;
        float ElapsedMilliseconds;
        Foundation::Time::Tick32::Representation FutureTick;
        bool SameClock;
        bool BoundAfterUnbind;
    };

    Result Run();

} // namespace ClockAndDuration
} // namespace Time
} // namespace FoundationExamples

#endif
