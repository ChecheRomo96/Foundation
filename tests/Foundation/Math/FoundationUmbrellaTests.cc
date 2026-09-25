#include <gtest/gtest.h>

#include <Foundation.h>

TEST(FoundationUmbrellaMathTest, ExposesMath) {
    EXPECT_EQ(Foundation::Math::GCD(54, 24), 6u);
}
