# Clock and Duration

## Scenario

A callback-backed clock produces two time points, their elapsed duration, and a future point.

## Interfaces

`Clock`, `Now`, `TimePoint`, `Duration`, `SameClock`, and `Unbind`.

## Run and inspect

Open `ClockAndDuration.ino` in Arduino IDE and set the serial monitor to **115200 baud**. The sketch prints the source expression, the concrete input, and an interpreted result; it is a demonstration rather than a PASS/FAIL test.

## Extend it

Change tick rate or readings and compare raw ticks with milliseconds.
