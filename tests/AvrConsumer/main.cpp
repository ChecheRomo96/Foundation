// ATmega328P firmware that links the exported Foundation package and checks
// representative APIs at runtime. It reports over USART0 (115200 8N1), lights
// the Uno LED (PB5) when every check passes, then halts with interrupts
// disabled.

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include <Foundation.h>
#include <Foundation/Time/Frequency.h>
#include <Foundation/Time/Period.h>

namespace {

    void UartInit() {
        // 16 MHz, double speed: UBRR 16 gives 115200 baud within 2.1%.
        UCSR0A = _BV(U2X0);
        UBRR0H = 0;
        UBRR0L = 16;
        UCSR0B = _BV(TXEN0);
        UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    }

    void UartPut(char c) {
        loop_until_bit_is_set(UCSR0A, UDRE0);
        UDR0 = static_cast<uint8_t>(c);
    }

    void UartPrint(const char* text) {
        while (*text != '\0') {
            UartPut(*text++);
        }
    }

    void UartPrint(uint8_t value) {
        char digits[3];
        uint8_t count = 0;
        do {
            digits[count++] = static_cast<char>('0' + value % 10);
            value = static_cast<uint8_t>(value / 10);
        } while (value != 0);
        while (count != 0) {
            UartPut(digits[--count]);
        }
    }

    uint8_t passed = 0;
    uint8_t total = 0;

    void Check(const char* name, bool ok) {
        ++total;
        if (ok) {
            ++passed;
        }
        UartPrint(ok ? "ok   " : "FAIL ");
        UartPrint(name);
        UartPrint("\r\n");
    }

    // Runtime inputs: volatile keeps the checks on the target CPU.
    volatile uint32_t hz = 1000;
    volatile int32_t negative = -13;
    volatile int32_t num = 6;
    volatile int32_t den = 8;

    int Add(int a, int b) {
        return a + b;
    }

    static_assert(
        Foundation::TypeTraits::is_same<uint32_t, unsigned long>::value,
        "avr-gcc uint32_t is unsigned long"
    );

} // namespace

int main() {
    UartInit();
    UartPrint("Foundation AVR consumer ");
    UartPrint(FOUNDATION_VERSION);
    UartPrint("\r\n");

    using Foundation::Math::Ratio;
    const Ratio reduced = Ratio(num, den).Reduced();
    Check("Ratio::Reduced 6/8 == 3/4",
        reduced.Numerator() == 3 && reduced.Denominator() == 4);
    Check("Ratio::Sign -3/4 == -1", Ratio(-num, den).Sign() == -1);
    Check("Math::GCD(30000, 1001) == 1",
        Foundation::Math::GCD(30000u, static_cast<uint32_t>(hz + 1)) == 1);
    Check("Math::FloorDiv(-13, 12) == -2",
        Foundation::Math::FloorDiv(negative, 12) == -2);
    Check("Math::FloorMod(-13, 12) == 11",
        Foundation::Math::FloorMod(negative, 12) == 11);

    // Out-of-line symbols from libFoundation.a.
    const Foundation::Time::Period period =
        Foundation::Time::Frequency(hz, 1).GetPeriod();
    Check("Frequency::GetPeriod 1000 Hz == 1/1000 s",
        period.Numerator() == 1 && period.Denominator() == 1000);
    const Foundation::Time::Frequency back = period.GetFrequency();
    Check("Period::GetFrequency round trip",
        back.Numerator() == 1000 && back.Denominator() == 1);
    Check("Frequency 0 Hz is invalid",
        !Foundation::Time::Frequency(hz - 1000, 1).GetPeriod().IsValid());

    int circularStorage[3] = {};
    Foundation::Containers::CircularBuffer<int> circular(circularStorage, 3);
    circular.Push(1);
    circular.Push(2);
    circular.Push(3);
    int value = 0;
    circular.Pop(value);
    circular.Push(4);
    Check("CircularBuffer FIFO wraps", value == 1 && !circular.IsEmpty());

    int queueStorage[2] = {};
    Foundation::Containers::Queue<int> queue(queueStorage, 2);
    queue.Push(10);
    queue.Push(20);
    queue.Pop(value);
    Check("Queue front == 10", value == 10);

    int stackStorage[2] = {};
    Foundation::Containers::Stack<int> stack(stackStorage, 2);
    stack.Push(5);
    stack.Push(6);
    stack.Pop(value);
    Check("Stack top == 6", value == 6 && stack.GetCount() == 1);

    Foundation::Functional::Callback<int, int, int> callback;
    Check("Callback unbound", !callback.IsBound());
    callback.Bind(Add);
    Check("Callback Invoke(4, 5) == 9", callback.Invoke(4, 5) == 9);

    int a = 1;
    int b = 2;
    Foundation::Utils::Swap(a, b);
    Check("Utils::Swap", a == 2 && b == 1);

    UartPrint(passed == total ? "FOUNDATION AVR CONSUMER: PASSED " :
                                "FOUNDATION AVR CONSUMER: FAILED ");
    UartPrint(passed);
    UartPrint("/");
    UartPrint(total);
    UartPrint("\r\n");
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UCSR0A |= _BV(TXC0);
    loop_until_bit_is_set(UCSR0A, TXC0);

    if (passed == total) {
        DDRB |= _BV(DDB5);
        PORTB |= _BV(PORTB5);
    }

    cli();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    for (;;) {
        sleep_cpu();
    }
}
