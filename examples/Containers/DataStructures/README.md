# CircularBuffer and BitVector

## Scenario

A producer/consumer FIFO wraps through fixed caller storage; a rhythm is packed into bits.

## Interfaces

`CircularBuffer<T>` is non-owning and never grows. `BitVector` may use supplied storage or own a CPSTL vector.

## Run and inspect

Open `DataStructures.ino` in Arduino IDE and set the serial monitor to **115200 baud**. The sketch prints the source expression, the concrete input, and an interpreted result; it is a demonstration rather than a PASS/FAIL test.

## Extend it

Change the capacity or rhythm pattern and observe wraparound, rejection at full capacity, and owned byte growth.
