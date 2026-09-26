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
    constexpr Tick32 compileTimeTick(32);
    static_assert(compileTimeTick.Value() == 32u);
    static_assert(noexcept(compileTimeTick.Value()));

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

TEST(TickTest, MutatesAndComparesRawValues) {
    Tick16 tick(10);
    const Tick16 equal(10);
    const Tick16 different(11);

    EXPECT_TRUE(tick == equal);
    EXPECT_FALSE(tick != equal);
    EXPECT_TRUE(tick != different);
    EXPECT_FALSE(tick == different);

    tick.SetValue(Tick16::MaximumValue());
    EXPECT_EQ(tick.Value(), Tick16::MaximumValue());
    EXPECT_EQ(tick, Tick16(Tick16::MaximumValue()));
}

TEST(TimeValueTest, SupportsConstantEvaluationWithoutExceptions) {
    constexpr Frequency frequency(1000, 1);
    constexpr Period period(1, 1000);
    constexpr Duration duration(5);
    constexpr TimePoint invalidPoint;

    static_assert(frequency.IsValid());
    static_assert(frequency.Hertz() == 1000.0f);
    static_assert(period.IsValid());
    static_assert(period.Milliseconds() == 1.0f);
    static_assert(duration.IsValid());
    static_assert(duration.Milliseconds(period) == 5.0f);
    static_assert(!invalidPoint.IsValid());
    static_assert(noexcept(frequency.GetPeriod()));
    static_assert(noexcept(period.GetFrequency()));
    static_assert(noexcept(duration + Duration(1)));
    static_assert(noexcept(invalidPoint - invalidPoint));

    SUCCEED();
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
    EXPECT_EQ(maximumFrequency.Numerator(), maximum);
    EXPECT_EQ(maximumFrequency.GetRatio().Numerator(), maximum);

    const Period maximumPeriod = maximumFrequency.GetPeriod();
    EXPECT_TRUE(maximumPeriod.IsValid());
    EXPECT_EQ(maximumPeriod.Numerator(), 1u);
    EXPECT_EQ(maximumPeriod.Denominator(), maximum);
    EXPECT_EQ(maximumPeriod.GetFrequency().Numerator(), maximum);

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

TEST(FrequencyTest, MutatesEachRatioTermAndItsReciprocal) {
    Frequency frequency(1000, 1);

    frequency.SetNumerator(0);
    EXPECT_FALSE(frequency.IsValid());
    frequency.SetNumerator(2000);
    EXPECT_TRUE(frequency.IsValid());
    EXPECT_FLOAT_EQ(frequency.Hertz(), 2000.0f);

    frequency.SetDenominator(0);
    EXPECT_FALSE(frequency.IsValid());
    frequency.SetDenominator(2);
    EXPECT_TRUE(frequency.IsValid());
    EXPECT_FLOAT_EQ(frequency.Hertz(), 1000.0f);

    frequency.Set(48000, 1);
    EXPECT_EQ(frequency.Numerator(), 48000u);
    EXPECT_EQ(frequency.Denominator(), 1u);
    EXPECT_NEAR(frequency.GetPeriod().Microseconds(), 20.833334f, 0.00001f);
}

TEST(PeriodTest, RoundTripsThroughFrequency) {
    const Period period = Frequency(1000, 1).GetPeriod();

    EXPECT_EQ(period.Numerator(), 1u);
    EXPECT_EQ(period.Denominator(), 1000u);
    EXPECT_NEAR(period.Seconds(), 0.001f, 0.000001f);
    EXPECT_EQ(period.GetFrequency().Numerator(), 1000u);
    EXPECT_EQ(period.GetFrequency().Denominator(), 1u);
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

TEST(PeriodTest, MutatesEachRatioTermAndConvertsMicroseconds) {
    Period period(1, 1000);

    period.SetNumerator(0);
    EXPECT_FALSE(period.IsValid());
    period.SetNumerator(1);
    EXPECT_TRUE(period.IsValid());

    period.SetDenominator(0);
    EXPECT_FALSE(period.IsValid());
    period.SetDenominator(2000);
    EXPECT_TRUE(period.IsValid());
    EXPECT_FLOAT_EQ(period.Microseconds(), 500.0f);

    period.Set(1, 48000);
    EXPECT_EQ(period.Numerator(), 1u);
    EXPECT_EQ(period.Denominator(), 48000u);
    EXPECT_NEAR(period.Microseconds(), 20.833334f, 0.00001f);
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

TEST(DurationTest, MutatesAndExercisesEveryComparisonOperator) {
    Duration duration(4);
    duration.SetTicks(5);
    EXPECT_EQ(duration.Ticks(), 5u);

    const Duration equal(5);
    const Duration lower(4);
    const Duration higher(6);
    EXPECT_TRUE(duration == equal);
    EXPECT_FALSE(duration != equal);
    EXPECT_TRUE(duration != lower);
    EXPECT_TRUE(lower < duration);
    EXPECT_TRUE(duration > lower);
    EXPECT_TRUE(lower <= duration);
    EXPECT_TRUE(duration <= equal);
    EXPECT_TRUE(duration >= lower);
    EXPECT_TRUE(duration >= equal);
    EXPECT_TRUE(higher > duration);

    duration.SetTicks(Tick32::HalfRange());
    EXPECT_FALSE(duration.IsValid());
    EXPECT_FALSE(duration < lower);
    EXPECT_FALSE(duration > lower);
    EXPECT_FALSE(duration <= lower);
    EXPECT_FALSE(duration >= lower);
}

TEST(ClockTest, ProducesTimePointsAndCanBeUnbound) {
    Clock clock(ReadTick, Frequency(1000, 1));
    ASSERT_TRUE(clock.IsBound());
    EXPECT_EQ(clock.GetFrequency().Numerator(), 1000u);

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

TEST(ClockTest, BindsAndMutatesItsFrequencyAndPeriod) {
    Clock clock;
    EXPECT_FALSE(clock.IsBound());

    CurrentTick = 64;
    clock.Bind(ReadTick);
    EXPECT_TRUE(clock.IsBound());
    EXPECT_EQ(clock.Now().Ticks(), 64u);

    clock.SetFrequency(Frequency(2000, 1));
    EXPECT_EQ(clock.GetFrequency().Numerator(), 2000u);
    EXPECT_EQ(clock.GetFrequency().Denominator(), 1u);
    EXPECT_FLOAT_EQ(clock.GetPeriod().Milliseconds(), 0.5f);

    clock.Bind(nullptr);
    EXPECT_FALSE(clock.IsBound());
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

TEST(TimePointTest, SubtractsDurationsAndRejectsInvalidDurations) {
    Clock clock(ReadTick, Frequency(1000, 1));
    const TimePoint point = clock.At(10);

    const TimePoint earlier = point - Duration(3);
    EXPECT_TRUE(earlier.IsValid());
    EXPECT_EQ(earlier.Ticks(), 7u);
    EXPECT_EQ(earlier.GetClock(), &clock);

    const TimePoint invalid = point - Duration::Invalid();
    EXPECT_FALSE(invalid.IsValid());
}

TEST(TimePointTest, TreatsTheExactHalfRangeAsAmbiguous) {
    using Clock8 = Foundation::Time::BasicClock<Tick8>;
    using TimePoint8 = Foundation::Time::BasicTimePoint<Tick8>;

    Clock8 clock(ReadTick8, Frequency(1000, 1));
    const TimePoint8 first = clock.At(0);
    const TimePoint8 same = clock.At(0);
    const TimePoint8 halfRange = clock.At(Tick8::HalfRange());

    EXPECT_TRUE(first == same);
    EXPECT_FALSE(first != same);
    EXPECT_TRUE(first <= same);
    EXPECT_TRUE(first >= same);

    EXPECT_FALSE(first == halfRange);
    EXPECT_TRUE(first != halfRange);
    EXPECT_FALSE(first < halfRange);
    EXPECT_FALSE(first > halfRange);
    EXPECT_FALSE(first <= halfRange);
    EXPECT_FALSE(first >= halfRange);
    EXPECT_FALSE((halfRange - first).IsValid());
    EXPECT_FALSE((first - halfRange).IsValid());
}
