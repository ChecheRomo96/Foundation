#include <gtest/gtest.h>

#include <Foundation_Time.h>

TEST(FoundationTimeHeaderTest, ExposesTime) {
    const Foundation::Time::Tick tick = 42;

    EXPECT_EQ(tick, 42u);
}
