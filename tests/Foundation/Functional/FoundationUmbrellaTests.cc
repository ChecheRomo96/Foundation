#include <gtest/gtest.h>

#include <Foundation.h>

namespace {

int IncrementFromFoundationUmbrella(int value) {
    return value + 1;
}

} // namespace

TEST(FoundationUmbrellaTest, ExposesFunctionalCallback) {
    Foundation::Functional::Callback<int, int> callback;
    callback.bind(IncrementFromFoundationUmbrella);

    EXPECT_EQ(callback.invoke(41), 42);
}
