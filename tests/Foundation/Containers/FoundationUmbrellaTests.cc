#include <gtest/gtest.h>

#include <Foundation.h>

TEST(FoundationUmbrellaContainersTest, ExposesContainers) {
    int storage[2] = {};
    Foundation::Containers::CircularBuffer<int> buffer(storage, 2);

    EXPECT_TRUE(buffer.Push(42));
}
