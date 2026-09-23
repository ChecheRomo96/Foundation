#include <gtest/gtest.h>

#include <Foundation/Functional.h>
#include <Foundation/Utils.h>

namespace {

int IncrementWithFunctionalFirst(int value) {
    return value + 1;
}

} // namespace

TEST(FunctionalUmbrellaTest, CoexistsWithUtilsWhenIncludedFirst) {
    Foundation::Functional::Callback<int, int> callback;
    callback.bind(IncrementWithFunctionalFirst);

    int first = 1;
    int second = 2;
    Foundation::Utils::Swap(first, second);

    EXPECT_EQ(callback.invoke(41), 42);
    EXPECT_EQ(first, 2);
    EXPECT_EQ(second, 1);
}
