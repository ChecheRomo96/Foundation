#include <gtest/gtest.h>

#include <Foundation.h>

TEST(FoundationUmbrellaTimeTest, ExposesTime) {
    const Foundation::Time::Tick tick = 42;

    EXPECT_EQ(tick, 42u);
}
