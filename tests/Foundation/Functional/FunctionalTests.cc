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

    int Read() {
        return total;
    }

    int total = 0;
};

} // namespace

using Foundation::Functional::Callback;

TEST(CallbackTest, BindsAndUnbindsAFreeFunction) {
    Callback<int, int, int> callback;
    EXPECT_FALSE(callback.status());

    callback.bind(Add);
    EXPECT_TRUE(callback.status());
    EXPECT_EQ(callback.invoke(4, 5), 9);

    callback.unbind();
    EXPECT_FALSE(callback.status());
}

TEST(CallbackTest, InvokesVoidAndNoArgumentFunctions) {
    Callback<void, int> voidCallback;
    voidCallback.bind(Store);
    voidCallback.invoke(23);
    EXPECT_EQ(LastValue, 23);

    Callback<int> noArgumentCallback;
    noArgumentCallback.bind(ReadConstant);
    EXPECT_EQ(noArgumentCallback.invoke(), 17);
}

TEST(CallbackTest, BindsMemberFunctionsWithArguments) {
    Accumulator accumulator;
    Callback<int, int> callback;
    callback.bind<Accumulator, &Accumulator::AddValue>(&accumulator);

    EXPECT_EQ(callback.invoke(6), 6);
    EXPECT_EQ(callback.invoke(4), 10);
}

TEST(CallbackTest, BindsVoidAndNoArgumentMemberFunctions) {
    Accumulator accumulator;
    Callback<void, int> resetCallback;
    resetCallback.bind<Accumulator, &Accumulator::ResetTo>(&accumulator);
    resetCallback.invoke(3);
    EXPECT_EQ(accumulator.total, 3);

    Callback<int> readCallback;
    readCallback.bind<Accumulator, &Accumulator::Read>(&accumulator);
    EXPECT_EQ(readCallback.invoke(), 3);
}
