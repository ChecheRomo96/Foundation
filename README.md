# Foundation

Foundation is a lightweight C++ foundation library for embedded and desktop projects.
It provides reusable low-level building blocks shared by projects such as MIDILAR and RoboFoundation.

## Modules

- `Containers`: CircularBuffer, Queue and Stack.
- `Math`: ratios, complex values, matrices, and arithmetic helpers.
- `TypeTraits`: embedded-oriented compile-time type inspection.
- `Functional`: callback utilities.
- `Time`: clocks, durations, frequencies, periods, ticks, and time points.
- `Scheduling`: Task, PeriodicTask, OneShotTask and TaskScheduler.
- `Utils`: minimal utility helpers such as move semantics.

## Build

CMake 3.25 or newer is required. Ninja is required by the Ninja presets, and
each cross-compiled preset requires its named compiler/runtime on `PATH` or in a
configured toolchain location. List the presets available on the current host:

```bash
cmake --list-presets=all
```

```bash
./scripts/build.sh macos_arm64 --config Debug
```

On Windows PowerShell:

```powershell
./scripts/build.ps1 windows_msvc_x64 -Configuration Debug
```

Configure a preset separately when target-specific cache values are needed;
subsequent workflows reuse its `build/<preset>` cache:

```bash
./scripts/configure.sh arm_none_eabi_cortex_m4f_hard -- \
  -DFOUNDATION_ARM_SYSROOT=/path/to/arm-none-eabi/sysroot
```

## Tests

```bash
./scripts/test.sh macos_arm64 --config Debug
```

The native CTest suite covers Math, TypeTraits, Utils, Containers, Functional,
Time, and Scheduling. GoogleTest is fetched only when testing is enabled, and
each module's tests are registered only when all required modules are enabled.
Examples remain separate API demonstrations.

## Export

Build and install one self-contained Release package under `dist/<preset>`:

```bash
./scripts/export.sh macos_arm64
./scripts/export.sh psoc5lp_armgcc_cortex_m3_soft
./scripts/export.sh atmega328p_avrgcc_avr5
```

Target presets define the platform, compiler, architecture, and ABI. Feature
options can be overridden for a custom export without creating another preset:

```bash
./scripts/export.sh macos_arm64 -- \
  -DFOUNDATION_SCHEDULING=OFF
```

Every default export contains shared public headers, one Release static library,
and CMake package files for `find_package(Foundation)`. Debug remains available
from the source build for development and testing, but is not distributed.

The full Doxygen workflow guide explains cache behavior, toolchains, package
compatibility, troubleshooting, and the Windows/macOS/STM32/AVR validation
matrix. Generate it with:

```bash
./scripts/docs.sh --fresh
```

Then open `build/documentation/docs/html/index.html` and navigate to
**Build Guide → Workflows**.

## Arduino

Copy this folder into your Arduino `libraries` folder and include:

```cpp
#include <Foundation.h>
```
