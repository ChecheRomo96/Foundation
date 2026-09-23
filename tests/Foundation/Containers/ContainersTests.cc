#include <gtest/gtest.h>

#include <Foundation/Containers.h>

using Foundation::Containers::CircularBuffer;
using Foundation::Containers::Queue;
using Foundation::Containers::Stack;

TEST(CircularBufferTest, PreservesFifoOrderAcrossWraparound) {
    int storage[3] = {};
    CircularBuffer<int> buffer(storage, 3);

    EXPECT_TRUE(buffer.IsEmpty());
    EXPECT_EQ(buffer.GetFreeSpace(), 3u);
    EXPECT_TRUE(buffer.Push(1));
    EXPECT_TRUE(buffer.Push(2));
    EXPECT_TRUE(buffer.Push(3));
    EXPECT_TRUE(buffer.IsFull());
    EXPECT_FALSE(buffer.Push(4));

    int value = 0;
    ASSERT_TRUE(buffer.Pop(value));
    EXPECT_EQ(value, 1);
    EXPECT_TRUE(buffer.Push(4));
    ASSERT_TRUE(buffer.Pop(value));
    EXPECT_EQ(value, 2);
    ASSERT_TRUE(buffer.Pop(value));
    EXPECT_EQ(value, 3);
    ASSERT_TRUE(buffer.Pop(value));
    EXPECT_EQ(value, 4);
    EXPECT_FALSE(buffer.Pop(value));
}

TEST(CircularBufferTest, ResetsItsState) {
    int storage[2] = {};
    CircularBuffer<int> buffer(storage, 2);
    ASSERT_TRUE(buffer.Push(8));

    buffer.Reset();

    EXPECT_TRUE(buffer.IsEmpty());
    EXPECT_EQ(buffer.GetAvailable(), 0u);
    EXPECT_EQ(buffer.GetFreeSpace(), 2u);
}

TEST(QueueTest, ExposesCircularBufferFifoBehavior) {
    int storage[2] = {};
    Queue<int> queue(storage, 2);
    int value = 0;

    ASSERT_TRUE(queue.Push(7));
    ASSERT_TRUE(queue.Pop(value));
    EXPECT_EQ(value, 7);
}

TEST(StackTest, PreservesLifoOrderAndCapacity) {
    int storage[2] = {};
    Stack<int> stack(storage, 2);
    int value = 0;

    EXPECT_TRUE(stack.Push(10));
    EXPECT_TRUE(stack.Push(20));
    EXPECT_TRUE(stack.IsFull());
    EXPECT_FALSE(stack.Push(30));
    ASSERT_TRUE(stack.Pop(value));
    EXPECT_EQ(value, 20);
    ASSERT_TRUE(stack.Pop(value));
    EXPECT_EQ(value, 10);
    EXPECT_TRUE(stack.IsEmpty());
}
