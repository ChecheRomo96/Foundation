#include <gtest/gtest.h>

#include <stdexcept>

#include <Foundation/Functional.h>

using Foundation::Functional::Callback;

TEST(CallbackExceptionTest, EmptyValueInvocationThrows) {
    const Callback<int, int> callback;

    EXPECT_THROW(callback.Invoke(42), std::runtime_error);
}

TEST(CallbackExceptionTest, EmptyVoidInvocationThrows) {
    const Callback<void, int> callback;

    EXPECT_THROW(callback.Invoke(42), std::runtime_error);
}
