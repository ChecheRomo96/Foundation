#include "Shared.h"

namespace FoundationExamples {
namespace Time {
namespace ClockAndDuration {

    namespace {

        Foundation::Time::Tick32::Representation CurrentTick = 0;

        Foundation::Time::Tick32::Representation ReadTick() {
            return CurrentTick;
        }

    } // namespace

    Result Run() {
        Foundation::Time::Clock clock(
            ReadTick,
            Foundation::Time::Frequency(1000, 1)
        );

        CurrentTick = 100;
        const Foundation::Time::TimePoint start = clock.Now();

        CurrentTick = 350;
        const Foundation::Time::TimePoint end = clock.Now();
        const Foundation::Time::Duration elapsed = end - start;
        const Foundation::Time::TimePoint future =
            end + Foundation::Time::Duration(50);

        Result result;
        result.StartingTick = start.Ticks();
        result.EndingTick = end.Ticks();
        result.ElapsedTicks = elapsed.Ticks();
        result.ElapsedMilliseconds = elapsed.Milliseconds(clock.GetPeriod());
        result.FutureTick = future.Ticks();
        result.SameClock = start.SameClock(end);

        clock.Unbind();
        result.BoundAfterUnbind = clock.IsBound();
        return result;
    }

} // namespace ClockAndDuration
} // namespace Time
} // namespace FoundationExamples
