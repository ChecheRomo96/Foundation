#include <gtest/gtest.h>

#include <Foundation/Utils.h>
#include <Foundation/Functional.h>

namespace {

int IncrementWithUtilsFirst(int value) {
    return value + 1;
}

} // namespace

TEST(FunctionalUmbrellaTest, CoexistsWithUtilsWhenIncludedSecond) {
    int first = 1;
    int second = 2;
    Foundation::Utils::Swap(first, second);

    Foundation::Functional::Callback<int, int> callback;
    callback.bind(IncrementWithUtilsFirst);

    EXPECT_EQ(first, 2);
    EXPECT_EQ(second, 1);
    EXPECT_EQ(callback.invoke(41), 42);
}
