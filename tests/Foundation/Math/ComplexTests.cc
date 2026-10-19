#include <gtest/gtest.h>

#include <Foundation/Math/Complex.h>

using Foundation::Math::Complex;

template<typename T> class SignedComplexTest : public ::testing::Test {};
using SignedScalars = ::testing::Types<int8_t, int16_t, int32_t, int64_t,
                                      float, double, long double>;
struct SignedScalarNames {
    template<typename T> static std::string GetName(int index) {
        const char* names[] = {"Int8", "Int16", "Int32", "Int64",
                               "Float", "Double", "LongDouble"};
        return names[index];
    }
};
TYPED_TEST_SUITE(SignedComplexTest, SignedScalars, SignedScalarNames);

TYPED_TEST(SignedComplexTest, PreservesNativeScalarArithmeticAndValueLayout) {
    using T = TypeParam;
    static_assert(__is_trivially_copyable(Complex<T>),
                  "Complex must be trivially copyable");
    static_assert(sizeof(Complex<T>) == 2 * sizeof(T), "Complex stores two scalars");
    const Complex<T> a(T(4), T(2)), b(T(1), T(-1));
    EXPECT_EQ(a + b, Complex<T>(T(5), T(1)));
    EXPECT_EQ(a - b, Complex<T>(T(3), T(3)));
    EXPECT_EQ(a * b, Complex<T>(T(6), T(-2)));
    EXPECT_EQ(a / b, Complex<T>(T(1), T(3)));
    EXPECT_EQ(-a, Complex<T>(T(-4), T(-2)));
    EXPECT_EQ(a.Conjugate(), Complex<T>(T(4), T(-2)));
    EXPECT_EQ(a.MagnitudeSquared(), T(20));
    Complex<T> value(a);
    value += b; EXPECT_EQ(value, a + b);
    value = a; value -= b; EXPECT_EQ(value, a - b);
    value = a; value *= b; EXPECT_EQ(value, a * b);
    value = a; value /= b; EXPECT_EQ(value, a / b);
    value = a; value *= value; EXPECT_EQ(value, a * a);
    value = a; value /= value; EXPECT_EQ(value, Complex<T>(T(1), T(0)));
}

TEST(ComplexTest, IntegerDivisionTruncatesTowardZero) {
    EXPECT_EQ(Complex<int8_t>(1, -1) / Complex<int8_t>(2, 0),
              Complex<int8_t>(0, 0));
    EXPECT_EQ(Complex<int16_t>(-3, 3) / Complex<int16_t>(2, 0),
              Complex<int16_t>(-1, 1));
}

TEST(ComplexTest, SmallIntegerDivisorUsesNativeIntegralPromotions) {
    // 12^2 + 12^2 = 288 fits the promoted int, but not int8_t.
    const Complex<int8_t> divisor(12, 12);
    EXPECT_EQ(divisor / divisor, Complex<int8_t>(1, 0));
    Complex<int8_t> value(divisor);
    value /= divisor;
    EXPECT_EQ(value, Complex<int8_t>(1, 0));
}

TEST(ComplexTest, LargeIntegersNeverRoundThroughFloat) {
    const int64_t exact = INT64_C(9007199254740993);
    const Complex<int64_t> value(exact, -exact);
    EXPECT_EQ(value + Complex<int64_t>(1, -1),
              Complex<int64_t>(exact + 1, -exact - 1));
    EXPECT_EQ(value * Complex<int64_t>(1, 0), value);
}

TEST(ComplexTest, ConstructsCopiesAndAssignsValues) {
    const Complex<int> zero;
    const Complex<int> realOnly(7);
    const Complex<int> parts(3, -4);
    const Complex<int> copy(parts);
    Complex<int> assigned;
    assigned = parts;

    EXPECT_EQ(zero.real, 0);
    EXPECT_EQ(zero.imag, 0);
    EXPECT_EQ(realOnly.real, 7);
    EXPECT_EQ(realOnly.imag, 0);
    EXPECT_EQ(copy, parts);
    EXPECT_EQ(assigned, parts);
}

TEST(ComplexTest, AppliesUnaryAndComparisonOperators) {
    const Complex<int> value(3, -4);
    const Complex<int> same(3, -4);
    const Complex<int> different(3, 4);

    EXPECT_EQ(+value, value);
    EXPECT_EQ(-value, Complex<int>(-3, 4));
    EXPECT_TRUE(value == same);
    EXPECT_FALSE(value != same);
    EXPECT_TRUE(value != different);
    EXPECT_FALSE(value == different);
}

TEST(ComplexTest, AppliesBinaryArithmetic) {
    const Complex<int> left(4, 2);
    const Complex<int> right(1, -1);

    EXPECT_EQ(left + right, Complex<int>(5, 1));
    EXPECT_EQ(left - right, Complex<int>(3, 3));
    EXPECT_EQ(left * right, Complex<int>(6, -2));
    EXPECT_EQ(left / right, Complex<int>(1, 3));
}

TEST(ComplexTest, AppliesCompoundArithmeticInPlace) {
    const Complex<int> operand(1, -1);

    Complex<int> sum(4, 2);
    EXPECT_EQ(&(sum += operand), &sum);
    EXPECT_EQ(sum, Complex<int>(5, 1));

    Complex<int> difference(4, 2);
    EXPECT_EQ(&(difference -= operand), &difference);
    EXPECT_EQ(difference, Complex<int>(3, 3));

    Complex<int> product(4, 2);
    EXPECT_EQ(&(product *= operand), &product);
    EXPECT_EQ(product, Complex<int>(6, -2));

    Complex<int> quotient(4, 2);
    EXPECT_EQ(&(quotient /= operand), &quotient);
    EXPECT_EQ(quotient, Complex<int>(1, 3));
}

TEST(ComplexTest, ComputesConjugateAndSquaredMagnitude) {
    const Complex<int> value(3, -4);

    EXPECT_EQ(value.Conjugate(), Complex<int>(3, 4));
    EXPECT_EQ(value.MagnitudeSquared(), 25);
    EXPECT_EQ(Complex<int>().MagnitudeSquared(), 0);
}

TEST(ComplexTest, PreservesFloatingPointDivisionResults) {
    const Complex<float> quotient =
        Complex<float>(1.0f, 2.0f) / Complex<float>(3.0f, -4.0f);

    EXPECT_NEAR(quotient.real, -0.2f, 0.000001f);
    EXPECT_NEAR(quotient.imag, 0.4f, 0.000001f);
}
