# TaskScheduler

## Scenario

A simulated clock drives one periodic and one one-shot task from caller-owned storage.

## Interfaces

`TaskScheduler`, `PeriodicTask`, `OneShotTask`, and `Update`.

## Run and inspect

Open `TaskScheduler.ino` in Arduino IDE and set the serial monitor to **115200 baud**. The sketch prints the source expression, the concrete input, and an interpreted result; it is a demonstration rather than a PASS/FAIL test.

## Extend it

Advance the simulated time or reduce capacity to see due-time and registration behaviour.
