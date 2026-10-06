#include <gtest/gtest.h>

#include <Foundation/Containers.h>

#include <utility>

using Foundation::Containers::BitVector;

TEST(BitVectorTest, StartsEmptyWithoutStorage) {
    BitVector bits;
    EXPECT_TRUE(bits.IsEmpty());
    EXPECT_EQ(bits.GetCapacity(), 0u);
    EXPECT_EQ(bits.Data(), nullptr);
    EXPECT_FALSE(bits.Get(0));
    EXPECT_FALSE(bits.Set(0, true));
    EXPECT_FALSE(bits.PopBack());
    EXPECT_EQ(BitVector::BytesFor(0), 0u);
    EXPECT_EQ(BitVector::BytesFor(1), 1u);
    EXPECT_EQ(BitVector::BytesFor(8), 1u);
    EXPECT_EQ(BitVector::BytesFor(9), 2u);
}

TEST(BitVectorTest, PacksEightBitsPerByte) {
    BitVector bits;
    const bool pattern[] = {true, false, false, true, false, false, true, false,
                            false, false, true};
    for (bool bit : pattern) {
        EXPECT_TRUE(bits.PushBack(bit));
    }
    EXPECT_EQ(bits.GetCount(), 11u);
    EXPECT_EQ(bits.GetByteCount(), 2u);
    EXPECT_EQ(bits.Data()[0], 0x49u);
    EXPECT_EQ(bits.Data()[1], 0x04u);
    EXPECT_EQ(bits.CountOnes(), 4u);
    for (size_t i = 0; i < 11; ++i) {
        EXPECT_EQ(bits.Get(i), pattern[i]);
    }
    EXPECT_FALSE(bits.Get(11));
}

TEST(BitVectorTest, SetsFlipsAndPops) {
    BitVector bits;
    bits.Resize(9);
    EXPECT_TRUE(bits.Set(8, true));
    EXPECT_TRUE(bits.Flip(0));
    EXPECT_FALSE(bits.Set(9, true));
    EXPECT_FALSE(bits.Flip(9));
    EXPECT_EQ(bits.CountOnes(), 2u);

    bits.FlipAll();
    EXPECT_EQ(bits.CountOnes(), 7u);
    EXPECT_EQ(bits.Data()[1], 0x00u);  // bits past the count stay zero

    bool last = true;
    EXPECT_TRUE(bits.PopBack(last));
    EXPECT_FALSE(last);
    EXPECT_EQ(bits.GetByteCount(), 1u);
    EXPECT_EQ(bits.GetCount(), 8u);
}

TEST(BitVectorTest, ResizesWithAValueAndClearsRemovedBits) {
    BitVector bits;
    EXPECT_TRUE(bits.Resize(3, true));
    EXPECT_EQ(bits.Data()[0], 0x07u);
    EXPECT_TRUE(bits.Resize(20, true));
    EXPECT_EQ(bits.CountOnes(), 20u);
    EXPECT_EQ(bits.Data()[2], 0x0Fu);
    EXPECT_TRUE(bits.Resize(5));
    EXPECT_EQ(bits.CountOnes(), 5u);
    EXPECT_EQ(bits.Data()[0], 0x1Fu);
    EXPECT_TRUE(bits.Resize(12));
    EXPECT_EQ(bits.CountOnes(), 5u);  // regrown bits are false
    EXPECT_GE(bits.GetCapacity(), 24u);
}

TEST(BitVectorTest, CapacityIsWholeBytes) {
    BitVector bits;
    EXPECT_TRUE(bits.Reserve(33));
    EXPECT_GE(bits.GetCapacity(), 40u);
    EXPECT_EQ(bits.GetCapacity() % 8u, 0u);
    EXPECT_TRUE(bits.IsEmpty());
    bits.Resize(10);
    EXPECT_TRUE(bits.ShrinkToFit());
    EXPECT_EQ(bits.GetCapacity(), 16u);
}

TEST(BitVectorTest, CopiesMovesAndCompares) {
    BitVector a;
    a.Resize(10);
    a.Set(3, true);
    BitVector b(a);
    EXPECT_EQ(a, b);
    b.Set(4, true);
    EXPECT_NE(a, b);

    BitVector c;
    c.Resize(10);
    EXPECT_NE(a, c);
    c = a;
    EXPECT_EQ(a, c);

    BitVector moved(std::move(b));
    EXPECT_TRUE(moved.Get(4));
    EXPECT_TRUE(b.IsEmpty());
    c = std::move(moved);
    EXPECT_TRUE(c.Get(4));
    EXPECT_TRUE(moved.IsEmpty());
}

TEST(BitVectorTest, ExternalStorageNeverGrows) {
    uint8_t storage[2] = {0xFFu, 0xFFu};
    BitVector bits(storage, sizeof storage);
    EXPECT_TRUE(bits.IsExternal());
    EXPECT_FALSE(bits.OwnsStorage());
    EXPECT_EQ(bits.Data(), storage);
    EXPECT_EQ(bits.GetCapacity(), 16u);

    EXPECT_TRUE(bits.Resize(12));
    EXPECT_EQ(storage[0], 0x00u);  // growing writes rests
    EXPECT_EQ(storage[1], 0x00u);
    EXPECT_TRUE(bits.Set(0, true));
    EXPECT_EQ(storage[0], 0x01u);

    EXPECT_FALSE(bits.Resize(17));
    EXPECT_FALSE(bits.Reserve(17));
    while (bits.GetCount() < 16u) {
        EXPECT_TRUE(bits.PushBack(true));
    }
    EXPECT_FALSE(bits.PushBack(true));
    EXPECT_EQ(bits.GetCount(), 16u);

    BitVector small;
    small.Resize(4, true);
    bits = small;
    EXPECT_FALSE(bits.OwnsStorage());
    EXPECT_EQ(storage[0], 0x0Fu);
    EXPECT_EQ(storage[1], 0x00u);

    BitVector large;
    large.Resize(17);
    EXPECT_FALSE(bits.Assign(large));
    bits = std::move(large);
    EXPECT_EQ(bits.GetCount(), 4u);
    EXPECT_EQ(large.GetCount(), 17u);

}

TEST(BitVectorTest, ANullPointerSelectsOwnedStorage) {
    BitVector bits(nullptr, 8);
    EXPECT_FALSE(bits.IsExternal());
    EXPECT_EQ(bits.GetCapacity(), 0u);
    for (int i = 0; i < 20; ++i) {
        EXPECT_TRUE(bits.PushBack(i % 2 == 0));
    }
    EXPECT_TRUE(bits.OwnsStorage());
    EXPECT_EQ(bits.CountOnes(), 10u);

    uint8_t storage[1] = {0xA5u};
    EXPECT_TRUE(bits.Attach(storage, sizeof storage));  // releases owned storage
    EXPECT_TRUE(bits.IsExternal());
    EXPECT_TRUE(bits.IsEmpty());
    EXPECT_EQ(storage[0], 0xA5u);  // attaching does not zero the buffer
    EXPECT_TRUE(bits.PushBack(true));
    EXPECT_EQ(storage[0] & 0x01u, 0x01u);

    EXPECT_TRUE(bits.Attach(nullptr, 1));  // back to empty owned storage
    EXPECT_FALSE(bits.IsExternal());
    EXPECT_TRUE(bits.IsEmpty());
    EXPECT_EQ(bits.Data(), nullptr);
    EXPECT_TRUE(bits.PushBack(true));
    EXPECT_TRUE(bits.OwnsStorage());

    EXPECT_TRUE(bits.Attach(storage, 0));  // a non-null pointer is used, even empty
    EXPECT_TRUE(bits.IsExternal());
    EXPECT_EQ(bits.GetCapacity(), 0u);
    EXPECT_FALSE(bits.PushBack(true));
}

TEST(BitVectorTest, RejectsAttachingItsOwnStorage) {
    BitVector bits;
    bits.Resize(8, true);
    uint8_t* own = const_cast<uint8_t*>(bits.Data());
    EXPECT_FALSE(bits.Attach(own, 1));
    EXPECT_EQ(bits.GetCount(), 8u);
    EXPECT_TRUE(bits.OwnsStorage());
}

TEST(BitVectorTest, ClearAndReleaseOnExternalStorage) {
    uint8_t storage[2] = {};
    BitVector bits(storage, sizeof storage);
    bits.Resize(10, true);
    EXPECT_EQ(storage[0], 0xFFu);
    EXPECT_EQ(storage[1], 0x03u);
    bits.Clear();
    EXPECT_TRUE(bits.IsEmpty());
    EXPECT_TRUE(bits.IsExternal());
    EXPECT_EQ(storage[0], 0x00u);
    EXPECT_EQ(storage[1], 0x00u);
    bits.Release();
    EXPECT_FALSE(bits.IsExternal());
    EXPECT_EQ(bits.GetCapacity(), 0u);
}

TEST(BitVectorTest, CopyOfExternalUsesOwnedStorage) {
    uint8_t storage[1] = {};
    BitVector external(storage, sizeof storage);
    external.Resize(5, true);
    BitVector copy(external);
    EXPECT_EQ(copy, external);
    EXPECT_FALSE(copy.IsExternal());
    EXPECT_NE(copy.Data(), storage);
    copy.Set(0, false);
    EXPECT_EQ(storage[0], 0x1Fu);

    BitVector moved(std::move(external));  // moving takes the external buffer
    EXPECT_TRUE(moved.IsExternal());
    EXPECT_EQ(moved.Data(), storage);
    EXPECT_FALSE(external.IsExternal());
    EXPECT_TRUE(external.IsEmpty());
}
