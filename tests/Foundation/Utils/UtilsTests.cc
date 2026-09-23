#include <gtest/gtest.h>

#include <Foundation/Utils.h>

namespace {

class MoveOnly {
public:
    explicit MoveOnly(int value)
        : value(value) {
    }

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&& other)
        : value(other.value) {
        other.value = 0;
    }

    MoveOnly& operator=(MoveOnly&& other) {
        value = other.value;
        other.value = 0;
        return *this;
    }

    int value;
};

} // namespace

TEST(MoveTest, TransfersAValueToANewObject) {
    MoveOnly source(42);
    MoveOnly destination(Foundation::Utils::Move(source));

    EXPECT_EQ(destination.value, 42);
    EXPECT_EQ(source.value, 0);
}

TEST(SwapTest, ExchangesTwoValues) {
    int first = 3;
    int second = 9;
    Foundation::Utils::Swap(first, second);

    EXPECT_EQ(first, 9);
    EXPECT_EQ(second, 3);
}
