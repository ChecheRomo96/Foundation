#include <gtest/gtest.h>

#include <stdexcept>

#include <Foundation/Utils.h>

namespace {

class MoveOnly {
public:
    explicit MoveOnly(int initial)
        : value(initial) {
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

class ThrowingMove {
public:
    static bool ThrowOnMove;

    explicit ThrowingMove(int initial)
        : value(initial) {
    }

    ThrowingMove(const ThrowingMove&) = delete;
    ThrowingMove& operator=(const ThrowingMove&) = delete;

    ThrowingMove(ThrowingMove&& other) {
        if(ThrowOnMove) {
            throw std::runtime_error("move construction failed");
        }

        value = other.value;
        other.value = 0;
    }

    ThrowingMove& operator=(ThrowingMove&& other) {
        if(ThrowOnMove) {
            throw std::runtime_error("move assignment failed");
        }

        value = other.value;
        other.value = 0;
        return *this;
    }

    int value;
};

bool ThrowingMove::ThrowOnMove = false;

} // namespace

TEST(MoveTest, TransfersAValueToANewObject) {
    constexpr int constantSource = 42;
    constexpr int constantResult = Foundation::Utils::Move(constantSource);
    static_assert(constantResult == 42);
    static_assert(noexcept(Foundation::Utils::Move(constantSource)));

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

TEST(SwapTest, ExchangesMoveOnlyValues) {
    MoveOnly first(3);
    MoveOnly second(9);

    Foundation::Utils::Swap(first, second);

    EXPECT_EQ(first.value, 9);
    EXPECT_EQ(second.value, 3);
}

TEST(UtilsTest, MoveIsOnlyACastAndSwapPropagatesElementFailures) {
    ThrowingMove source(42);
    static_assert(noexcept(Foundation::Utils::Move(source)));

    ThrowingMove::ThrowOnMove = true;
    EXPECT_NO_THROW(Foundation::Utils::Move(source));

    ThrowingMove other(17);
    EXPECT_THROW(
        Foundation::Utils::Swap(source, other),
        std::runtime_error
    );
    EXPECT_EQ(source.value, 42);
    EXPECT_EQ(other.value, 17);
    ThrowingMove::ThrowOnMove = false;
}
