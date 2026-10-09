# Flash data

## Scenario

The same source reads read-only tables on desktop and program memory on AVR.

## Interfaces

`FOUNDATION_FLASH`, `Flash::Read`, and `Flash::CopyString`.

## Run and inspect

Open `FlashData.ino` in Arduino IDE and set the serial monitor to **115200 baud**. The sketch prints the source expression, the concrete input, and an interpreted result; it is a demonstration rather than a PASS/FAIL test.

## Extend it

Change the destination buffer size and inspect the copied/truncated string.
