#include <gtest/gtest.h>

#include <Foundation_Functional.h>

namespace {

int IncrementFromTopLevelFunctionalHeader(int value) {
    return value + 1;
}

} // namespace

TEST(FoundationFunctionalHeaderTest, ExposesFunctionalCallback) {
    Foundation::Functional::Callback<int, int> callback;
    callback.bind(IncrementFromTopLevelFunctionalHeader);

    EXPECT_EQ(callback.invoke(41), 42);
}
