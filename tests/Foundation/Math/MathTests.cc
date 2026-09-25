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
    static_assert(GCD(54, 24) == 6u, "GCD must be constexpr");
    static_assert(noexcept(GCD(54, 24)), "GCD must not throw");

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
