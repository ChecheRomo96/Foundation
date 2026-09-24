#include <gtest/gtest.h>

#include <Foundation_Functional.h>

namespace {

int IncrementFromTopLevelFunctionalHeader(int value) {
    return value + 1;
}

} // namespace

TEST(FoundationFunctionalHeaderTest, ExposesFunctionalCallback) {
    Foundation::Functional::Callback<int, int> callback;
    callback.Bind(IncrementFromTopLevelFunctionalHeader);

    EXPECT_EQ(callback.Invoke(41), 42);
}
