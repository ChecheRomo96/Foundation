#include <gtest/gtest.h>

#include <limits>
#include <type_traits>

#include <Foundation/Time.h>

namespace {

Foundation::Time::Tick32::Representation CurrentTick = 0;
Foundation::Time::Tick8::Representation CurrentTick8 = 0;

Foundation::Time::Tick32::Representation ReadTick() {
    return CurrentTick;
}

Foundation::Time::Tick8::Representation ReadTick8() {
    return CurrentTick8;
}

} // namespace

using Foundation::Time::Clock;
using Foundation::Time::Duration;
using Foundation::Time::Frequency;
using Foundation::Time::Period;
using Foundation::Time::Tick8;
using Foundation::Time::Tick16;
using Foundation::Time::Tick32;
using Foundation::Time::Tick64;
using Foundation::Time::TimePoint;

TEST(TickTest, ProvidesExplicitUnsignedWidths) {
    static_assert(
        Foundation::TypeTraits::is_same<
            Tick32,
            Foundation::Time::Tick<uint32_t>
        >::value,
        "Tick32 must alias Tick<uint32_t>"
    );
    static_assert(
        Foundation::TypeTraits::is_same<
            Tick32,
            Foundation::Time::Tick<>
        >::value,
        "Tick<> must default to Tick32"
    );

    const Tick8 tick8(8);
    const Tick16 tick16(16);
    const Tick32 tick32(32);
    const Tick64 tick64(64);

    EXPECT_EQ(tick8.Value(), 8u);
    EXPECT_EQ(tick16.Value(), 16u);
    EXPECT_EQ(tick32.Value(), 32u);
    EXPECT_EQ(tick64.Value(), 64u);
    EXPECT_EQ(Tick8::HalfRange(), 128u);
    EXPECT_EQ(Tick16::HalfRange(), 32768u);
}

TEST(FrequencyTest, ConvertsFrequencyAndPeriodUnits) {
    const Frequency frequency(1000, 1);

    EXPECT_TRUE(frequency.IsValid());
    EXPECT_FLOAT_EQ(frequency.Hertz(), 1000.0f);
    EXPECT_NEAR(frequency.PeriodSeconds(), 0.001f, 0.000001f);
    EXPECT_FLOAT_EQ(frequency.PeriodMilliseconds(), 1.0f);
    EXPECT_FLOAT_EQ(frequency.PeriodMicroseconds(), 1000.0f);
}

TEST(FrequencyTest, RejectsZeroTermsAndPreservesUnsignedValues) {
    constexpr uint32_t maximum = std::numeric_limits<uint32_t>::max();
    const Foundation::Math::UnsignedRatio ratio(maximum, 1);
    const Frequency maximumFrequency(ratio);

    EXPECT_TRUE(maximumFrequency.IsValid());
    EXPECT_EQ(maximumFrequency.Num(), maximum);
    EXPECT_EQ(maximumFrequency.GetRatio().Num(), maximum);

    const Period maximumPeriod = maximumFrequency.GetPeriod();
    EXPECT_TRUE(maximumPeriod.IsValid());
    EXPECT_EQ(maximumPeriod.Num(), 1u);
    EXPECT_EQ(maximumPeriod.Den(), maximum);
    EXPECT_EQ(maximumPeriod.GetFrequency().Num(), maximum);

    const Frequency zeroFrequency(0, 1);
    const Frequency zeroDenominator(1, 0);
    EXPECT_FALSE(Frequency().IsValid());
    EXPECT_FALSE(zeroFrequency.IsValid());
    EXPECT_FALSE(zeroDenominator.IsValid());
    EXPECT_FLOAT_EQ(zeroFrequency.Hertz(), 0.0f);
    EXPECT_FLOAT_EQ(zeroFrequency.PeriodSeconds(), 0.0f);
    EXPECT_FALSE(zeroFrequency.GetPeriod().IsValid());
    EXPECT_FALSE(zeroDenominator.GetPeriod().IsValid());
}

TEST(PeriodTest, RoundTripsThroughFrequency) {
    const Period period = Frequency(1000, 1).GetPeriod();

    EXPECT_EQ(period.Num(), 1u);
    EXPECT_EQ(period.Den(), 1000u);
    EXPECT_NEAR(period.Seconds(), 0.001f, 0.000001f);
    EXPECT_EQ(period.GetFrequency().Num(), 1000u);
    EXPECT_EQ(period.GetFrequency().Den(), 1u);
}

TEST(PeriodTest, RejectsZeroTermsAndInvalidReciprocals) {
    const Period zeroPeriod(0, 1);
    const Period zeroDenominator(1, 0);

    EXPECT_FALSE(Period().IsValid());
    EXPECT_FALSE(zeroPeriod.IsValid());
    EXPECT_FALSE(zeroDenominator.IsValid());
    EXPECT_FLOAT_EQ(zeroPeriod.Seconds(), 0.0f);
    EXPECT_FLOAT_EQ(zeroPeriod.Milliseconds(), 0.0f);
    EXPECT_FALSE(zeroPeriod.GetFrequency().IsValid());
    EXPECT_FALSE(zeroDenominator.GetFrequency().IsValid());
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

TEST(DurationTest, RejectsUnderflowOverflowAndOutOfRangeValues) {
    EXPECT_FALSE((Duration(2) - Duration(3)).IsValid());
    EXPECT_FALSE(
        (Duration(Duration::MaximumTicks()) + Duration(1)).IsValid()
    );
    EXPECT_FALSE(Duration(Tick32::HalfRange()).IsValid());
    EXPECT_TRUE(Duration(0).IsValid());
    EXPECT_TRUE(Duration(0).IsZero());
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

TEST(ClockTest, KeepsObjectIdentityStable) {
    static_assert(!std::is_copy_constructible<Clock>::value);
    static_assert(!std::is_copy_assignable<Clock>::value);
    static_assert(!std::is_move_constructible<Clock>::value);
    static_assert(!std::is_move_assignable<Clock>::value);
    static_assert(
        !std::is_constructible<
            TimePoint,
            TimePoint::Representation,
            const Clock*
        >::value
    );

    Clock clock(ReadTick, Frequency(1000, 1));
    const TimePoint point = clock.At(125);

    EXPECT_TRUE(point.IsValid());
    EXPECT_EQ(point.Ticks(), 125u);
    EXPECT_EQ(point.GetClock(), &clock);
}

TEST(TimePointTest, ComparesOnlyPointsFromTheSameClock) {
    Clock clock(ReadTick, Frequency(1000, 1));
    const TimePoint now = clock.At(125);
    const TimePoint later = now + Duration(10);

    EXPECT_GT(later, now);
    EXPECT_EQ((later - now).Ticks(), 10u);

    Clock otherClock(ReadTick, Frequency(1000, 1));
    const TimePoint other = otherClock.At(125);
    EXPECT_FALSE(now.SameClock(other));
    EXPECT_FALSE((now - other).IsValid());
    EXPECT_FALSE((now < other));
    EXPECT_FALSE((now >= other));
}

TEST(TimePointTest, HandlesCounterWrapWithinTheHalfRange) {
    using Clock8 = Foundation::Time::BasicClock<Tick8>;
    using Duration8 = Foundation::Time::BasicDuration<Tick8>;
    using TimePoint8 = Foundation::Time::BasicTimePoint<Tick8>;

    Clock8 clock(ReadTick8, Frequency(1000, 1));

    CurrentTick8 = 254;
    const TimePoint8 beforeWrap = clock.Now();
    CurrentTick8 = 2;
    const TimePoint8 afterWrap = clock.Now();

    const Duration8 elapsed = afterWrap - beforeWrap;
    EXPECT_TRUE(elapsed.IsValid());
    EXPECT_EQ(elapsed.Ticks(), 4u);
    EXPECT_GT(afterWrap, beforeWrap);
    EXPECT_LT(beforeWrap, afterWrap);
    EXPECT_FALSE((beforeWrap - afterWrap).IsValid());
}
