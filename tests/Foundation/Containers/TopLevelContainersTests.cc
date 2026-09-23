#include <gtest/gtest.h>

#include <Foundation_Containers.h>

TEST(FoundationContainersHeaderTest, ExposesContainers) {
    int storage[2] = {};
    Foundation::Containers::Stack<int> stack(storage, 2);

    EXPECT_TRUE(stack.Push(42));
}
