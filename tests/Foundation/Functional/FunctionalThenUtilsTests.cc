#include <gtest/gtest.h>

#include <stdint.h>

#include <Foundation/Functional.h>
#include <Foundation/Utils.h>

namespace {

int IncrementWithFunctionalFirst(int value) {
    return value + 1;
}

} // namespace

TEST(FunctionalUmbrellaTest, CoexistsWithUtilsWhenIncludedFirst) {
    Foundation::Functional::Callback<int, int> callback;
    callback.Bind(IncrementWithFunctionalFirst);

    const uint8_t source[2] = {1, 2};
    uint8_t copy[2] = {0, 0};
    Foundation::Utils::Flash::Copy(copy, source, sizeof copy);

    EXPECT_EQ(callback.Invoke(41), 42);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
}
