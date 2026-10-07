#include <gtest/gtest.h>

#include <Foundation_Containers.h>

TEST(FoundationContainersHeaderTest, ExposesContainers) {
    Foundation::Containers::BitVector bits;

    EXPECT_TRUE(bits.PushBack(true));
    EXPECT_TRUE(bits.Get(0));
}
