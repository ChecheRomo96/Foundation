#include <gtest/gtest.h>

#include <cstdint>
#include <limits>
#include <type_traits>

#include <Foundation/Math.h>

using Foundation::Math::GCD;

#if defined(FOUNDATION_MATH_RATIO)
using Foundation::Math::BasicRatio;
using Foundation::Math::Ratio;
using Foundation::Math::UnsignedRatio;
#endif

TEST(ArithmeticTest, CalculatesGreatestCommonDivisor) {
    EXPECT_EQ(GCD(54, 24), 6u);
    EXPECT_EQ(GCD(0, 7), 7u);
}

#if defined(FOUNDATION_MATH_RATIO)
TEST(RatioTest, SupportsConstantConstructionAndConversion) {
    static_assert(std::is_same<Ratio, BasicRatio<int32_t>>::value);
    static_assert(
        std::is_same<UnsignedRatio, BasicRatio<uint32_t>>::value
    );

    constexpr Ratio half(1, 2);
    static_assert(half.Num() == 1, "Ratio numerator must be constexpr");
    static_assert(half.Den() == 2, "Ratio denominator must be constexpr");
    static_assert(half.IsValid(), "A nonzero denominator must be valid");

    EXPECT_FLOAT_EQ(half.ToFloat(), 0.5f);
}

TEST(RatioTest, PreservesTheFullUnsignedRange) {
    constexpr uint32_t maximum = std::numeric_limits<uint32_t>::max();
    constexpr UnsignedRatio maximumRatio(maximum, 1);
    static_assert(maximumRatio.Num() == maximum);
    static_assert(maximumRatio.Den() == 1);
    static_assert(maximumRatio.IsValid());

    const UnsignedRatio reduced = UnsignedRatio(42, 56).Reduced();
    EXPECT_EQ(reduced.Num(), 3u);
    EXPECT_EQ(reduced.Den(), 4u);
    EXPECT_EQ(reduced.Sign(), 1);
}

TEST(RatioTest, ReducesWithoutLosingTheSign) {
    const Ratio reduced = Ratio(-42, 56).Reduced();

    EXPECT_EQ(reduced.Num(), -3);
    EXPECT_EQ(reduced.Den(), 4);
    EXPECT_EQ(reduced.Sign(), -1);
}

TEST(RatioTest, ReducesInPlace) {
    Ratio ratio(-42, 56);
    ratio.Reduce();

    EXPECT_EQ(ratio.Num(), -3);
    EXPECT_EQ(ratio.Den(), 4);
    EXPECT_EQ(ratio.Sign(), -1);

    ratio.Set(-5, -10);
    ratio.Reduce();

    EXPECT_EQ(ratio.Num(), -1);
    EXPECT_EQ(ratio.Den(), -2);
    EXPECT_EQ(ratio.Sign(), 1);
}

TEST(RatioTest, ReducesMinimumSignedValuesWithoutOverflow) {
    constexpr std::int32_t minimum =
        std::numeric_limits<std::int32_t>::min();

    const Ratio minimumNumerator = Ratio(minimum, 2).Reduced();
    EXPECT_EQ(minimumNumerator.Num(), minimum / 2);
    EXPECT_EQ(minimumNumerator.Den(), 1);

    const Ratio minimumDenominator = Ratio(2, minimum).Reduced();
    EXPECT_EQ(minimumDenominator.Num(), 1);
    EXPECT_EQ(minimumDenominator.Den(), minimum / 2);
}

TEST(RatioTest, HandlesAnInvalidDenominator) {
    const Ratio invalid(1, 0);
    const Ratio reduced = invalid.Reduced();

    EXPECT_FALSE(invalid.IsValid());
    EXPECT_FLOAT_EQ(invalid.ToFloat(), 0.0f);
    EXPECT_EQ(reduced.Num(), 0);
    EXPECT_EQ(reduced.Den(), 1);
}
#endif
