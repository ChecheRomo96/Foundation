#include <gtest/gtest.h>

#include <Foundation.h>

TEST(FoundationUmbrellaContainersTest, ExposesContainers) {
    int storage[2] = {};
    Foundation::Containers::Queue<int> queue(storage, 2);

    EXPECT_TRUE(queue.Push(42));
}
