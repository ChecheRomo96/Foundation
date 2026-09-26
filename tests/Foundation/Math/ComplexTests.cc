#include <gtest/gtest.h>

#include <Foundation/Math/Complex.h>

using Foundation::Math::Complex;

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
