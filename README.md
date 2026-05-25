# Foundation

Foundation is a lightweight C++ foundation library for embedded and desktop projects.
It provides reusable low-level building blocks shared by projects such as MIDILAR and RoboFoundation.

## Modules

- `Containers`: CircularBuffer, Queue and Stack.
- `Time`: Clock abstraction.
- `Scheduling`: Task, PeriodicTask, OneShotTask and TaskScheduler.
- `Utils`: minimal utility helpers such as move semantics.

## Build

```bash
cmake --preset debug
cmake --build --preset debug
```

## Tests

```bash
cmake --preset tests
cmake --build --preset tests
ctest --test-dir build/tests --output-on-failure
```

## Arduino

Copy this folder into your Arduino `libraries` folder and include:

```cpp
#include <Foundation.h>
```
