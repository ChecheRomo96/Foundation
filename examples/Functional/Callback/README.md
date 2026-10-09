# Callback

## Scenario

A small dispatcher binds both an ordinary function and a method on a stateful object.

## Interfaces

`Bind`, `Invoke`, `IsBound`, and `Unbind` provide one non-owning callback interface.

## Run and inspect

Open `Callback.ino` in Arduino IDE and set the serial monitor to **115200 baud**. The sketch prints the source expression, the concrete input, and an interpreted result; it is a demonstration rather than a PASS/FAIL test.

## Extend it

Bind a different object method and keep that object alive longer than the callback.
