#include <gtest/gtest.h>

#include <Foundation/Containers.h>

using Foundation::Containers::CircularBuffer;

namespace {

class TrackedValue {
public:
    static int CopyAssignments;
    static int MoveAssignments;

    explicit TrackedValue(int value = 0)
        : Value(value) {
    }

    TrackedValue(const TrackedValue& other)
        : Value(other.Value) {
    }

    TrackedValue& operator=(const TrackedValue& other) {
        Value = other.Value;
        ++CopyAssignments;
        return *this;
    }

    TrackedValue& operator=(TrackedValue&& other) noexcept {
        Value = other.Value;
        other.Value = 0;
        ++MoveAssignments;
        return *this;
    }

    static void ResetCounts() {
        CopyAssignments = 0;
        MoveAssignments = 0;
    }

    int Value;
};

int TrackedValue::CopyAssignments = 0;
int TrackedValue::MoveAssignments = 0;

} // namespace

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

TEST(CircularBufferTest, CopiesAnExplicitLvalue) {
    TrackedValue::ResetCounts();
    TrackedValue storage[1];
    CircularBuffer<TrackedValue> buffer(storage, 1);
    const TrackedValue source(42);

    ASSERT_TRUE(buffer.Push(source));
    EXPECT_EQ(TrackedValue::CopyAssignments, 1);
    EXPECT_EQ(TrackedValue::MoveAssignments, 0);

    TrackedValue result;
    ASSERT_TRUE(buffer.Pop(result));
    EXPECT_EQ(result.Value, 42);
    EXPECT_EQ(source.Value, 42);
    EXPECT_EQ(TrackedValue::MoveAssignments, 1);
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

