#include <gtest/gtest.h>

#include <Foundation/Containers.h>

using Foundation::Containers::CircularBuffer;
using Foundation::Containers::Queue;
using Foundation::Containers::Stack;

TEST(CircularBufferTest, PreservesFifoOrderAcrossWraparound) {
    int storage[3] = {};
    CircularBuffer<int> buffer(storage, 3);

    EXPECT_TRUE(buffer.IsValid());
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

    EXPECT_TRUE(stack.IsValid());
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

TEST(CircularBufferTest, SafelyRejectsNullStorageWithNonzeroCapacity) {
    CircularBuffer<int> buffer(nullptr, 3);
    int value = 99;

    EXPECT_FALSE(buffer.IsValid());
    EXPECT_TRUE(buffer.IsEmpty());
    EXPECT_TRUE(buffer.IsFull());
    EXPECT_EQ(buffer.GetSize(), 3u);
    EXPECT_EQ(buffer.GetFreeSpace(), 0u);
    EXPECT_FALSE(buffer.Push(1));
    EXPECT_FALSE(buffer.Pop(value));
    EXPECT_EQ(value, 99);

    buffer.Reset();
    EXPECT_EQ(buffer.GetAvailable(), 0u);
}

TEST(CircularBufferTest, AcceptsNullStorageForZeroCapacity) {
    CircularBuffer<int> buffer(nullptr, 0);
    int value = 99;

    EXPECT_TRUE(buffer.IsValid());
    EXPECT_TRUE(buffer.IsEmpty());
    EXPECT_TRUE(buffer.IsFull());
    EXPECT_EQ(buffer.GetFreeSpace(), 0u);
    EXPECT_FALSE(buffer.Push(1));
    EXPECT_FALSE(buffer.Pop(value));
    EXPECT_EQ(value, 99);
}

TEST(StackTest, SafelyRejectsNullStorageWithNonzeroCapacity) {
    Stack<int> stack(nullptr, 3);
    int value = 99;

    EXPECT_FALSE(stack.IsValid());
    EXPECT_TRUE(stack.IsEmpty());
    EXPECT_TRUE(stack.IsFull());
    EXPECT_EQ(stack.GetCapacity(), 3u);
    EXPECT_FALSE(stack.Push(1));
    EXPECT_FALSE(stack.Pop(value));
    EXPECT_EQ(value, 99);

    stack.Reset();
    EXPECT_EQ(stack.GetCount(), 0u);
}

TEST(StackTest, AcceptsNullStorageForZeroCapacity) {
    Stack<int> stack(nullptr, 0);
    int value = 99;

    EXPECT_TRUE(stack.IsValid());
    EXPECT_TRUE(stack.IsEmpty());
    EXPECT_TRUE(stack.IsFull());
    EXPECT_FALSE(stack.Push(1));
    EXPECT_FALSE(stack.Pop(value));
    EXPECT_EQ(value, 99);
}
