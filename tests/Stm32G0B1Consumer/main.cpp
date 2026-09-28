#include <stdint.h>

#include <Foundation.h>
#include <Foundation/Time/Frequency.h>
#include <Foundation/Time/Period.h>

extern "C" {

    extern uint32_t _sidata;
    extern uint32_t _sdata;
    extern uint32_t _edata;
    extern uint32_t _sbss;
    extern uint32_t _ebss;

    volatile uint32_t FoundationValidationState;
    volatile uint32_t FoundationValidationPassed;
    volatile uint32_t FoundationValidationTotal;

    int main();

}

namespace {

    constexpr uint32_t ValidationRunning = UINT32_C(0x52554E21);
    constexpr uint32_t ValidationPassed = UINT32_C(0x50415353);
    constexpr uint32_t ValidationFailed = UINT32_C(0x4641494C);

    volatile uint32_t hertz = 1000;
    volatile int32_t negative = -13;
    volatile int32_t numerator = 6;
    volatile int32_t denominator = 8;

    void Check(bool condition) {
        ++FoundationValidationTotal;
        if (condition) {
            ++FoundationValidationPassed;
        }
    }

    int Add(int left, int right) {
        return left + right;
    }

}

extern "C" __attribute__((noreturn)) void FoundationValidationHalt() {
    __asm volatile("bkpt #0");
    for (;;) {
        __asm volatile("wfi");
    }
}

extern "C" __attribute__((noreturn)) void Reset_Handler() {
    uint32_t* source = &_sidata;
    for (uint32_t* destination = &_sdata;
         destination < &_edata;
         ++destination) {
        *destination = *source++;
    }

    for (uint32_t* destination = &_sbss;
         destination < &_ebss;
         ++destination) {
        *destination = 0;
    }

    (void)main();
    FoundationValidationHalt();
}

int main() {
    FoundationValidationState = ValidationRunning;
    FoundationValidationPassed = 0;
    FoundationValidationTotal = 0;

    using Foundation::Math::Ratio;

    const Ratio reduced = Ratio(numerator, denominator).Reduced();
    Check(reduced.Numerator() == 3 && reduced.Denominator() == 4);
    Check(Ratio(-numerator, denominator).Sign() == -1);
    Check(Ratio(1, 2).Value<int32_t>() == 0);
    Check(Foundation::Math::GCD(30000u, hertz + 1) == 1);
    Check(Foundation::Math::FloorDiv(negative, 12) == -2);
    Check(Foundation::Math::FloorMod(negative, 12) == 11);

    const Foundation::Time::Period period =
        Foundation::Time::Frequency(hertz, 1).GetPeriod();
    Check(period.Numerator() == 1 && period.Denominator() == 1000);

    const Foundation::Time::Frequency frequency = period.GetFrequency();
    Check(frequency.Numerator() == 1000 && frequency.Denominator() == 1);

    int circularStorage[3] = {};
    Foundation::Containers::CircularBuffer<int> circular(
        circularStorage,
        3
    );
    circular.Push(1);
    circular.Push(2);
    circular.Push(3);
    int value = 0;
    circular.Pop(value);
    circular.Push(4);
    Check(value == 1 && !circular.IsEmpty());

    int queueStorage[2] = {};
    Foundation::Containers::Queue<int> queue(queueStorage, 2);
    queue.Push(10);
    queue.Push(20);
    queue.Pop(value);
    Check(value == 10);

    int stackStorage[2] = {};
    Foundation::Containers::Stack<int> stack(stackStorage, 2);
    stack.Push(5);
    stack.Push(6);
    stack.Pop(value);
    Check(value == 6 && stack.GetCount() == 1);

    Foundation::Functional::Callback<int, int, int> callback;
    Check(!callback.IsBound());
    callback.Bind(Add);
    Check(callback.Invoke(4, 5) == 9);

    int left = 1;
    int right = 2;
    Foundation::Utils::Swap(left, right);
    Check(left == 2 && right == 1);

    FoundationValidationState =
        (FoundationValidationPassed == FoundationValidationTotal)
            ? ValidationPassed
            : ValidationFailed;

    FoundationValidationHalt();
}
