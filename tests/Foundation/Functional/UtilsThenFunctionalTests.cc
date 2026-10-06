#include <gtest/gtest.h>

#include <stdint.h>

#include <Foundation/Utils.h>
#include <Foundation/Functional.h>

namespace {

int IncrementWithUtilsFirst(int value) {
    return value + 1;
}

} // namespace

TEST(FunctionalUmbrellaTest, CoexistsWithUtilsWhenIncludedSecond) {
    const uint8_t source[2] = {1, 2};
    uint8_t copy[2] = {0, 0};
    Foundation::Utils::Flash::Copy(copy, source, sizeof copy);

    Foundation::Functional::Callback<int, int> callback;
    callback.Bind(IncrementWithUtilsFirst);

    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(callback.Invoke(41), 42);
}
