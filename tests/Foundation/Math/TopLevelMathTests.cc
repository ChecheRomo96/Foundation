#include <gtest/gtest.h>

#include <Foundation_Math.h>

TEST(FoundationMathHeaderTest, ExposesMath) {
    EXPECT_EQ(Foundation::Math::GCD(54, 24), 6u);
}
