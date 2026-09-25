#include <gtest/gtest.h>

#include <Foundation.h>

namespace {

int IncrementFromFoundationUmbrella(int value) {
    return value + 1;
}

} // namespace

TEST(FoundationUmbrellaTest, ExposesFunctionalCallback) {
    Foundation::Functional::Callback<int, int> callback;
    callback.Bind(IncrementFromFoundationUmbrella);

    EXPECT_EQ(callback.Invoke(41), 42);
}
