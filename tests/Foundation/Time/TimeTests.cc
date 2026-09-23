#include <gtest/gtest.h>

#include <Foundation/Time.h>

namespace {

Foundation::Time::Tick CurrentTick = 0;

Foundation::Time::Tick ReadTick() {
    return CurrentTick;
}

} // namespace

using Foundation::Time::Clock;
using Foundation::Time::Duration;
using Foundation::Time::Frequency;
using Foundation::Time::Period;
using Foundation::Time::TimePoint;

TEST(FrequencyTest, ConvertsFrequencyAndPeriodUnits) {
    const Frequency frequency(1000, 1);

    EXPECT_TRUE(frequency.IsValid());
    EXPECT_FLOAT_EQ(frequency.Hertz(), 1000.0f);
    EXPECT_NEAR(frequency.PeriodSeconds(), 0.001f, 0.000001f);
    EXPECT_FLOAT_EQ(frequency.PeriodMilliseconds(), 1.0f);
    EXPECT_FLOAT_EQ(frequency.PeriodMicroseconds(), 1000.0f);
}

TEST(PeriodTest, RoundTripsThroughFrequency) {
    const Period period = Frequency(1000, 1).GetPeriod();

    EXPECT_EQ(period.Num(), 1u);
    EXPECT_EQ(period.Den(), 1000u);
    EXPECT_NEAR(period.Seconds(), 0.001f, 0.000001f);
    EXPECT_EQ(period.GetFrequency().Num(), 1000u);
    EXPECT_EQ(period.GetFrequency().Den(), 1u);
}

TEST(DurationTest, SupportsConversionsArithmeticAndComparison) {
    const Period period(1, 1000);
    const Duration duration(5);

    EXPECT_EQ(duration.Ticks(), 5u);
    EXPECT_FLOAT_EQ(duration.Milliseconds(period), 5.0f);
    EXPECT_EQ((duration + Duration(3)).Ticks(), 8u);
    EXPECT_EQ((duration - Duration(2)).Ticks(), 3u);
    EXPECT_GT(duration, Duration(4));
}

TEST(ClockTest, ProducesTimePointsAndCanBeUnbound) {
    Clock clock(ReadTick, Frequency(1000, 1));
    ASSERT_TRUE(clock.IsBound());
    EXPECT_EQ(clock.GetFrequency().Num(), 1000u);

    CurrentTick = 125;
    const TimePoint now = clock.Now();
    EXPECT_TRUE(now.IsValid());
    EXPECT_EQ(now.Ticks(), 125u);
    EXPECT_EQ(now.GetClock(), &clock);

    clock.Unbind();
    EXPECT_FALSE(clock.IsBound());
    EXPECT_EQ(clock.Now().Ticks(), 0u);
}

TEST(TimePointTest, ComparesOnlyPointsFromTheSameClock) {
    Clock clock(ReadTick, Frequency(1000, 1));
    const TimePoint now(125, &clock);
    const TimePoint later = now + Duration(10);

    EXPECT_GT(later, now);
    EXPECT_EQ((later - now).Ticks(), 10u);

    Clock otherClock(ReadTick, Frequency(1000, 1));
    const TimePoint other(125, &otherClock);
    EXPECT_FALSE(now.SameClock(other));
    EXPECT_EQ((now - other).Ticks(), 0u);
}
