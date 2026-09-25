#include <gtest/gtest.h>

#include <Foundation/Functional.h>

namespace {

int Add(int lhs, int rhs) {
    return lhs + rhs;
}

int ReadConstant() {
    return 17;
}

int LastValue = 0;
int ConstInvocationCount = 0;

void Store(int value) {
    LastValue = value;
}

class Accumulator {
public:
    int AddValue(int value) {
        total += value;
        return total;
    }

    void ResetTo(int value) {
        total = value;
    }

    int Read() const {
        return total;
    }

    void Touch() const {
        ++ConstInvocationCount;
    }

    int total = 0;
};

} // namespace

using Foundation::Functional::Callback;

TEST(CallbackTest, BindsAndUnbindsAFreeFunction) {
    Callback<int, int, int> callback;
    EXPECT_FALSE(callback.IsBound());

    callback.Bind(Add);
    EXPECT_TRUE(callback.IsBound());
    EXPECT_EQ(callback.Invoke(4, 5), 9);

    callback.Unbind();
    EXPECT_FALSE(callback.IsBound());
}

TEST(CallbackTest, InvokesVoidAndNoArgumentFunctions) {
    Callback<void, int> voidCallback;
    voidCallback.Bind(Store);
    voidCallback.Invoke(23);
    EXPECT_EQ(LastValue, 23);

    Callback<int> noArgumentCallback;
    noArgumentCallback.Bind(ReadConstant);
    EXPECT_EQ(noArgumentCallback.Invoke(), 17);
}

TEST(CallbackTest, BindsMemberFunctionsWithArguments) {
    Accumulator accumulator;
    Callback<int, int> callback;
    callback.Bind<Accumulator, &Accumulator::AddValue>(&accumulator);

    EXPECT_EQ(callback.Invoke(6), 6);
    EXPECT_EQ(callback.Invoke(4), 10);
}

TEST(CallbackTest, BindsVoidAndNoArgumentMemberFunctions) {
    Accumulator accumulator;
    Callback<void, int> resetCallback;
    resetCallback.Bind<Accumulator, &Accumulator::ResetTo>(&accumulator);
    resetCallback.Invoke(3);
    EXPECT_EQ(accumulator.total, 3);

    Callback<int> readCallback;
    readCallback.Bind<Accumulator, &Accumulator::Read>(&accumulator);
    EXPECT_EQ(readCallback.Invoke(), 3);
}

TEST(CallbackTest, SupportsVoidCallbacksWithoutArguments) {
    const Accumulator accumulator;
    Callback<void> callback;

    callback.Bind<Accumulator, &Accumulator::Touch>(&accumulator);
    callback.Invoke();

    EXPECT_EQ(ConstInvocationCount, 1);
}

TEST(CallbackTest, RejectsNullMemberInstances) {
    Callback<int, int> callback;
    callback.Bind<Accumulator, &Accumulator::AddValue>(nullptr);

    EXPECT_FALSE(callback.IsBound());
}
