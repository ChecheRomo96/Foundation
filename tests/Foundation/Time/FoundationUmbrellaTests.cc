#include <gtest/gtest.h>

#include <Foundation.h>

TEST(FoundationUmbrellaTimeTest, ExposesTime) {
    const Foundation::Time::Tick32 tick = 42;

    EXPECT_EQ(tick.Value(), 42u);
}
