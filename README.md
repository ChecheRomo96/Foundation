# Foundation

Foundation is a lightweight C++ foundation library for embedded and desktop projects.
It provides reusable low-level building blocks shared by projects such as MIDILAR and RoboFoundation.

**Documentation:** [Foundation on GitHub Pages](https://checheromo96.github.io/Foundation/)
· [Support matrix](https://checheromo96.github.io/Foundation/group__Foundation__BuildGuide__SupportMatrix.html)
· [Known limitations](https://checheromo96.github.io/Foundation/group__Foundation__BuildGuide__KnownLimitations.html)
· [Changelog](CHANGELOG.md)

Foundation 1.2.0 is the current published release. Its five native GitHub
Release packages passed the tagged build, test, export, checksum, and standalone
package-consumer workflow. Other presets remain candidates or experimental
until their documented validation gates pass.

Foundation 1.2.0 requires a C++17-capable compiler in every integration mode,
including CMake packages, direct source builds, and Arduino-style source builds.
The language requirement does not imply a dependency on the complete C++
standard library.

## Modules

- `Containers`: CircularBuffer, Queue and Stack.
- `Math`: ratios, complex values, matrices, and arithmetic helpers.
- `TypeTraits`: embedded-oriented compile-time type inspection.
- `Functional`: callback utilities.
- `Time`: clocks, durations, frequencies, periods, ticks, and time points.
- `Scheduling`: Task, PeriodicTask, OneShotTask and TaskScheduler.
- `Utils`: minimal utility helpers such as move semantics.

## Build

CMake 3.25 or newer and C++17 are required. Ninja is required by the Ninja presets, and
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

The [published Doxygen documentation](https://checheromo96.github.io/Foundation/)
is the primary guide for API usage, cache behavior, toolchains, package
compatibility, troubleshooting, known limitations, and the
Windows/macOS/Linux/STM32/AVR validation matrix.

Generate the same documentation locally with:

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

## License

Copyright (c) 2026 José Manuel Romo. All rights reserved.

Foundation is currently proprietary and is not open-source software. No
permission is granted for external use, compilation, modification,
redistribution, integration, or commercial use without prior written
authorization. See [LICENSE](LICENSE) for the complete notice.

The long-term licensing and distribution model still requires review by
qualified legal counsel. Publishing source, documentation, or binary artifacts
does not grant permission to use them. Until the license is replaced in
writing, the all-rights-reserved notice applies.
