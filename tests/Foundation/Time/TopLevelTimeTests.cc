#include <gtest/gtest.h>

#include <Foundation_Time.h>

TEST(FoundationTimeHeaderTest, ExposesTime) {
    const Foundation::Time::Tick32 tick = 42;

    EXPECT_EQ(tick.Value(), 42u);
}
