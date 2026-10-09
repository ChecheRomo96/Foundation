# Ratio

## Scenario

A signed rational value is preserved exactly before conversion to a selected numeric type.

## Interfaces

`Ratio`, validity, exact numerator/denominator, and `Value<TResult>()`.

## Run and inspect

Open `Ratio.ino` in Arduino IDE and set the serial monitor to **115200 baud**. The sketch prints the source expression, the concrete input, and an interpreted result; it is a demonstration rather than a PASS/FAIL test.

## Extend it

Try a zero denominator and verify that the example labels the invalid state.
