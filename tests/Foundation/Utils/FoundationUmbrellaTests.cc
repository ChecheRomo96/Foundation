#include <gtest/gtest.h>

#include <Foundation.h>

TEST(FoundationUmbrellaUtilsTest, ExposesUtils) {
    int first = 1;
    int second = 2;
    Foundation::Utils::Swap(first, second);

    EXPECT_EQ(first, 2);
    EXPECT_EQ(second, 1);
}
