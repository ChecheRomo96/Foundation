#include <gtest/gtest.h>

#include <cstdint>
#include <limits>
#include <type_traits>

#include <Foundation/Math.h>

using Foundation::Math::FloorDiv;
using Foundation::Math::FloorMod;
using Foundation::Math::GCD;

#if defined(FOUNDATION_MATH_RATIO)
using Foundation::Math::BasicRatio;
using Foundation::Math::Ratio;
using Foundation::Math::UnsignedRatio;
#endif

TEST(ArithmeticTest, CalculatesGreatestCommonDivisor) {
    static_assert(GCD(54, 24) == 6u, "GCD must be constexpr");
    static_assert(noexcept(GCD(54, 24)), "GCD must not throw");

    EXPECT_EQ(GCD(54, 24), 6u);
    EXPECT_EQ(GCD(0, 7), 7u);
}

TEST(ArithmeticTest, FloorModIsConstexprAndNoexcept) {
    static_assert(FloorMod(-1, 12) == 11, "FloorMod must be constexpr");
    static_assert(FloorMod(-12, 12) == 0, "FloorMod must be constexpr");
    static_assert(FloorMod(7, 0) == 0, "FloorMod must be constexpr");
    static_assert(noexcept(FloorMod(-1, 12)), "FloorMod must not throw");
    static_assert(
        std::is_same<decltype(FloorMod(0, 1)), std::int32_t>::value,
        "FloorMod must return int32_t"
    );

    constexpr std::int32_t pitchClass = FloorMod(-13, 12);
    static_assert(pitchClass == 11, "FloorMod must be usable in constants");
}

TEST(ArithmeticTest, FloorModHandlesPositiveNegativeAndZeroValues) {
    EXPECT_EQ(FloorMod(0, 12), 0);
    EXPECT_EQ(FloorMod(5, 12), 5);
    EXPECT_EQ(FloorMod(11, 12), 11);
    EXPECT_EQ(FloorMod(13, 12), 1);
    EXPECT_EQ(FloorMod(-1, 12), 11);
    EXPECT_EQ(FloorMod(-11, 12), 1);
    EXPECT_EQ(FloorMod(-13, 12), 11);
    EXPECT_EQ(FloorMod(-7, 3), 2);
}

TEST(ArithmeticTest, FloorModReturnsZeroForExactMultiples) {
    EXPECT_EQ(FloorMod(12, 12), 0);
    EXPECT_EQ(FloorMod(24, 12), 0);
    EXPECT_EQ(FloorMod(-12, 12), 0);
    EXPECT_EQ(FloorMod(-24, 12), 0);
    EXPECT_EQ(FloorMod(-36, 12), 0);
}

TEST(ArithmeticTest, FloorModByOneIsAlwaysZero) {
    constexpr std::int32_t minimum =
        std::numeric_limits<std::int32_t>::min();
    constexpr std::int32_t maximum =
        std::numeric_limits<std::int32_t>::max();

    EXPECT_EQ(FloorMod(0, 1), 0);
    EXPECT_EQ(FloorMod(17, 1), 0);
    EXPECT_EQ(FloorMod(-17, 1), 0);
    EXPECT_EQ(FloorMod(minimum, 1), 0);
    EXPECT_EQ(FloorMod(maximum, 1), 0);
}

TEST(ArithmeticTest, FloorModHandlesExtremeValuesWithoutOverflow) {
    constexpr std::int32_t minimum =
        std::numeric_limits<std::int32_t>::min();
    constexpr std::int32_t maximum =
        std::numeric_limits<std::int32_t>::max();

    static_assert(FloorMod(minimum, 12) == 4);
    static_assert(FloorMod(maximum, 12) == 7);

    EXPECT_EQ(FloorMod(minimum, 12), 4);
    EXPECT_EQ(FloorMod(maximum, 12), 7);
    EXPECT_EQ(FloorMod(minimum, 2), 0);
    EXPECT_EQ(FloorMod(maximum, 2), 1);
    EXPECT_EQ(FloorMod(minimum, maximum), maximum - 1);
    EXPECT_EQ(FloorMod(maximum, maximum), 0);
    EXPECT_EQ(FloorMod(minimum + 1, maximum), 0);
    EXPECT_EQ(FloorMod(-1, maximum), maximum - 1);
}

TEST(ArithmeticTest, FloorModReturnsZeroForNonPositiveModulus) {
    constexpr std::int32_t minimum =
        std::numeric_limits<std::int32_t>::min();
    constexpr std::int32_t maximum =
        std::numeric_limits<std::int32_t>::max();

    static_assert(FloorMod(minimum, -1) == 0);

    EXPECT_EQ(FloorMod(5, 0), 0);
    EXPECT_EQ(FloorMod(-5, 0), 0);
    EXPECT_EQ(FloorMod(5, -3), 0);
    EXPECT_EQ(FloorMod(-5, -3), 0);
    EXPECT_EQ(FloorMod(minimum, -1), 0);
    EXPECT_EQ(FloorMod(maximum, -1), 0);
    EXPECT_EQ(FloorMod(minimum, minimum), 0);
    EXPECT_EQ(FloorMod(maximum, minimum), 0);
}

TEST(ArithmeticTest, FloorModMatchesReferenceExhaustively) {
    for(std::int32_t modulus = 1; modulus <= 13; ++modulus) {
        for(std::int32_t value = -200; value <= 200; ++value) {
            const std::int64_t wide =
                static_cast<std::int64_t>(value) % modulus;
            const std::int64_t expected = (wide + modulus) % modulus;
            const std::int32_t actual = FloorMod(value, modulus);

            ASSERT_EQ(actual, expected)
                << "value=" << value << " modulus=" << modulus;
            ASSERT_GE(actual, 0);
            ASSERT_LT(actual, modulus);
        }
    }
}

TEST(ArithmeticTest, FloorDivIsConstexprAndNoexcept) {
    static_assert(FloorDiv(-1, 12) == -1, "FloorDiv must be constexpr");
    static_assert(FloorDiv(-12, 12) == -1, "FloorDiv must be constexpr");
    static_assert(FloorDiv(-13, 12) == -2, "FloorDiv must be constexpr");
    static_assert(FloorDiv(7, 0) == 0, "FloorDiv must be constexpr");
    static_assert(noexcept(FloorDiv(-1, 12)), "FloorDiv must not throw");
    static_assert(
        std::is_same<decltype(FloorDiv(0, 1)), std::int32_t>::value,
        "FloorDiv must return int32_t"
    );

    constexpr std::int32_t octave = FloorDiv(-13, 12);
    static_assert(octave == -2, "FloorDiv must be usable in constants");
}

TEST(ArithmeticTest, FloorDivHandlesPositiveNegativeAndZeroValues) {
    EXPECT_EQ(FloorDiv(0, 12), 0);
    EXPECT_EQ(FloorDiv(5, 12), 0);
    EXPECT_EQ(FloorDiv(11, 12), 0);
    EXPECT_EQ(FloorDiv(12, 12), 1);
    EXPECT_EQ(FloorDiv(13, 12), 1);
    EXPECT_EQ(FloorDiv(-1, 12), -1);
    EXPECT_EQ(FloorDiv(-11, 12), -1);
    EXPECT_EQ(FloorDiv(-12, 12), -1);
    EXPECT_EQ(FloorDiv(-13, 12), -2);
    EXPECT_EQ(FloorDiv(-7, 3), -3);
}

TEST(ArithmeticTest, FloorDivHandlesExactMultiples) {
    EXPECT_EQ(FloorDiv(12, 12), 1);
    EXPECT_EQ(FloorDiv(24, 12), 2);
    EXPECT_EQ(FloorDiv(-12, 12), -1);
    EXPECT_EQ(FloorDiv(-24, 12), -2);
    EXPECT_EQ(FloorDiv(-36, 12), -3);
}

TEST(ArithmeticTest, FloorDivByOneReturnsTheDividend) {
    constexpr std::int32_t minimum =
        std::numeric_limits<std::int32_t>::min();
    constexpr std::int32_t maximum =
        std::numeric_limits<std::int32_t>::max();

    static_assert(FloorDiv(minimum, 1) == minimum);
    static_assert(FloorDiv(maximum, 1) == maximum);

    EXPECT_EQ(FloorDiv(0, 1), 0);
    EXPECT_EQ(FloorDiv(17, 1), 17);
    EXPECT_EQ(FloorDiv(-17, 1), -17);
    EXPECT_EQ(FloorDiv(minimum, 1), minimum);
    EXPECT_EQ(FloorDiv(maximum, 1), maximum);
}

TEST(ArithmeticTest, FloorDivHandlesExtremeValuesWithoutOverflow) {
    constexpr std::int32_t minimum =
        std::numeric_limits<std::int32_t>::min();
    constexpr std::int32_t maximum =
        std::numeric_limits<std::int32_t>::max();

    static_assert(FloorDiv(minimum, 12) == -178956971);
    static_assert(FloorDiv(maximum, 12) == 178956970);

    EXPECT_EQ(FloorDiv(minimum, 12), -178956971);
    EXPECT_EQ(FloorDiv(maximum, 12), 178956970);
    EXPECT_EQ(FloorDiv(minimum, 2), -1073741824);
    EXPECT_EQ(FloorDiv(maximum, 2), 1073741823);
    EXPECT_EQ(FloorDiv(minimum, maximum), -2);
    EXPECT_EQ(FloorDiv(maximum, maximum), 1);
    EXPECT_EQ(FloorDiv(minimum + 1, maximum), -1);
    EXPECT_EQ(FloorDiv(-1, maximum), -1);
}

TEST(ArithmeticTest, FloorDivReturnsZeroForNonPositiveDivisor) {
    constexpr std::int32_t minimum =
        std::numeric_limits<std::int32_t>::min();
    constexpr std::int32_t maximum =
        std::numeric_limits<std::int32_t>::max();

    static_assert(FloorDiv(minimum, -1) == 0);

    EXPECT_EQ(FloorDiv(5, 0), 0);
    EXPECT_EQ(FloorDiv(-5, 0), 0);
    EXPECT_EQ(FloorDiv(5, -3), 0);
    EXPECT_EQ(FloorDiv(-5, -3), 0);
    EXPECT_EQ(FloorDiv(minimum, -1), 0);
    EXPECT_EQ(FloorDiv(maximum, -1), 0);
    EXPECT_EQ(FloorDiv(minimum, minimum), 0);
    EXPECT_EQ(FloorDiv(maximum, minimum), 0);
}

TEST(ArithmeticTest, FloorDivMatchesReferenceAndFloorModInvariantExhaustively) {
    for(std::int32_t divisor = 1; divisor <= 13; ++divisor) {
        for(std::int32_t value = -200; value <= 200; ++value) {
            const std::int64_t wideValue = value;
            const std::int64_t wideDivisor = divisor;
            std::int64_t expected = wideValue / wideDivisor;
            if((wideValue % wideDivisor) < 0) {
                --expected;
            }

            const std::int32_t actual = FloorDiv(value, divisor);
            const std::int64_t reconstructed =
                static_cast<std::int64_t>(actual) * wideDivisor +
                FloorMod(value, divisor);

            ASSERT_EQ(actual, expected)
                << "value=" << value << " divisor=" << divisor;
            ASSERT_EQ(reconstructed, wideValue)
                << "value=" << value << " divisor=" << divisor;
        }
    }
}

#if defined(FOUNDATION_MATH_RATIO)
TEST(RatioTest, SupportsConstantConstructionAndConversion) {
    static_assert(std::is_same<Ratio, BasicRatio<int32_t>>::value);
    static_assert(
        std::is_same<UnsignedRatio, BasicRatio<uint32_t>>::value
    );

    constexpr Ratio half(1, 2);
    constexpr Ratio reduced = Ratio(-42, 56).Reduced();
    static_assert(half.Numerator() == 1, "Ratio numerator must be constexpr");
    static_assert(half.Denominator() == 2, "Ratio denominator must be constexpr");
    static_assert(half.IsValid(), "A nonzero denominator must be valid");
    static_assert(reduced.Numerator() == -3, "Ratio reduction must be constexpr");
    static_assert(reduced.Denominator() == 4, "Ratio reduction must be constexpr");
    static_assert(noexcept(half.ToFloat()), "Ratio value operations must not throw");

    EXPECT_FLOAT_EQ(half.ToFloat(), 0.5f);
}

TEST(RatioTest, PreservesTheFullUnsignedRange) {
    constexpr uint32_t maximum = std::numeric_limits<uint32_t>::max();
    constexpr UnsignedRatio maximumRatio(maximum, 1);
    static_assert(maximumRatio.Numerator() == maximum);
    static_assert(maximumRatio.Denominator() == 1);
    static_assert(maximumRatio.IsValid());

    const UnsignedRatio reduced = UnsignedRatio(42, 56).Reduced();
    EXPECT_EQ(reduced.Numerator(), 3u);
    EXPECT_EQ(reduced.Denominator(), 4u);
    EXPECT_EQ(reduced.Sign(), 1);
}

TEST(RatioTest, ReducesWithoutLosingTheSign) {
    const Ratio reduced = Ratio(-42, 56).Reduced();

    EXPECT_EQ(reduced.Numerator(), -3);
    EXPECT_EQ(reduced.Denominator(), 4);
    EXPECT_EQ(reduced.Sign(), -1);
}

TEST(RatioTest, ReducesInPlace) {
    Ratio ratio(-42, 56);
    ratio.Reduce();

    EXPECT_EQ(ratio.Numerator(), -3);
    EXPECT_EQ(ratio.Denominator(), 4);
    EXPECT_EQ(ratio.Sign(), -1);

    ratio.Set(-5, -10);
    ratio.Reduce();

    EXPECT_EQ(ratio.Numerator(), -1);
    EXPECT_EQ(ratio.Denominator(), -2);
    EXPECT_EQ(ratio.Sign(), 1);
}

TEST(RatioTest, UpdatesNumeratorAndDenominatorIndependently) {
    constexpr Ratio updated = [] {
        Ratio ratio;
        ratio.SetNumerator(-3);
        ratio.SetDenominator(4);
        return ratio;
    }();
    static_assert(updated.Numerator() == -3);
    static_assert(updated.Denominator() == 4);
    static_assert(updated.IsValid());

    Ratio ratio(1, 2);
    ratio.SetNumerator(5);
    EXPECT_EQ(ratio.Numerator(), 5);
    EXPECT_EQ(ratio.Denominator(), 2);

    ratio.SetDenominator(0);
    EXPECT_FALSE(ratio.IsValid());
    EXPECT_FLOAT_EQ(ratio.ToFloat(), 0.0f);

    ratio.SetDenominator(-10);
    EXPECT_TRUE(ratio.IsValid());
    EXPECT_EQ(ratio.Sign(), -1);
    EXPECT_FLOAT_EQ(ratio.ToFloat(), -0.5f);
}

TEST(RatioTest, HandlesZeroAndExtremeTerms) {
    constexpr std::int32_t minimum =
        std::numeric_limits<std::int32_t>::min();
    constexpr std::int32_t maximum =
        std::numeric_limits<std::int32_t>::max();

    const Ratio zero = Ratio(0, maximum).Reduced();
    EXPECT_EQ(zero.Numerator(), 0);
    EXPECT_EQ(zero.Denominator(), 1);
    EXPECT_EQ(zero.Sign(), 0);

    const Ratio equalMinimums = Ratio(minimum, minimum).Reduced();
    EXPECT_EQ(equalMinimums.Numerator(), -1);
    EXPECT_EQ(equalMinimums.Denominator(), -1);
    EXPECT_EQ(equalMinimums.Sign(), 1);
    EXPECT_FLOAT_EQ(equalMinimums.ToFloat(), 1.0f);

    const Ratio coprimeLimits = Ratio(maximum, minimum).Reduced();
    EXPECT_EQ(coprimeLimits.Numerator(), maximum);
    EXPECT_EQ(coprimeLimits.Denominator(), minimum);
    EXPECT_EQ(coprimeLimits.Sign(), -1);
}

TEST(RatioTest, ReducesMinimumSignedValuesWithoutOverflow) {
    constexpr std::int32_t minimum =
        std::numeric_limits<std::int32_t>::min();

    const Ratio minimumNumerator = Ratio(minimum, 2).Reduced();
    EXPECT_EQ(minimumNumerator.Numerator(), minimum / 2);
    EXPECT_EQ(minimumNumerator.Denominator(), 1);

    const Ratio minimumDenominator = Ratio(2, minimum).Reduced();
    EXPECT_EQ(minimumDenominator.Numerator(), 1);
    EXPECT_EQ(minimumDenominator.Denominator(), minimum / 2);
}

TEST(RatioTest, HandlesAnInvalidDenominator) {
    const Ratio invalid(1, 0);
    const Ratio reduced = invalid.Reduced();

    EXPECT_FALSE(invalid.IsValid());
    EXPECT_FLOAT_EQ(invalid.ToFloat(), 0.0f);
    EXPECT_EQ(reduced.Numerator(), 0);
    EXPECT_EQ(reduced.Denominator(), 1);
}
#endif
