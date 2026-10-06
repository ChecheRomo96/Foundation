# Foundation

Foundation is a lightweight C++ foundation library for embedded and desktop projects.
It provides reusable low-level building blocks shared by projects such as MIDILAR and RoboFoundation.

**Documentation:** [Foundation on GitHub Pages](https://checheromo96.github.io/Foundation/)
· [Support matrix](https://checheromo96.github.io/Foundation/group__Foundation__BuildGuide__SupportMatrix.html)
· [Known limitations](https://checheromo96.github.io/Foundation/group__Foundation__BuildGuide__KnownLimitations.html)
· [Changelog](CHANGELOG.md)

Foundation 1.4.0 is the current published release. Its five native GitHub
Release packages passed the tagged build, test, export, checksum, and standalone
package-consumer workflow. Other presets remain candidates or experimental
until their documented validation gates pass. The unreleased 2.0.0 on `main`
builds on CPSTL; see [CHANGELOG](CHANGELOG.md).

Foundation requires a C++17-capable compiler for CMake packages and direct
source builds. Arduino source builds (`ARDUINO` defined) accept C++11 so stock
Arduino cores work unmodified. The language requirement does not imply a
dependency on the complete C++ standard library.

## CPSTL

Foundation 2.0.0 builds on [CPSTL](https://github.com/ChecheRomo96/CPSTL)
1.1.0 or newer for the standard vocabulary: `cpstd::vector`, `cpstd::move`,
`cpstd::swap`, `cpstd::stack`, `cpstd::queue` and the type traits. `cpstd`
aliases `std` in CPSTL's STL mode and is CPSTL's own implementation elsewhere,
such as AVR. Foundation does not reimplement standard types; it adds what the
standard library lacks.

CMake resolves CPSTL from a parent project, `FOUNDATION_CPSTL_PREFIX`, a
sibling export at `../CPSTL/dist/<preset>`, the normal package search, or,
by default, its sources at tag `v1.1.0` (`-DFETCHCONTENT_SOURCE_DIR_CPSTL=../CPSTL`
uses a local working copy). Arduino users install the CPSTL library next to
Foundation; `library.properties` declares it.

## Checkout

Foundation pins its reusable toolchains, hidden preset bases, and generic
configure/build/test/install/clean engine through the `tools/RoModularBuild`
Git submodule. Clone the complete source tree with:

```bash
git clone --recurse-submodules https://github.com/ChecheRomo96/Foundation.git
```

For an existing checkout, initialize the pinned infrastructure revision before
running CMake or any repository workflow:

```bash
git submodule update --init --recursive
```

`RoModularBuild` is publicly readable so clean clones and GitHub-hosted runners
can obtain the pinned files without a cross-repository secret. Public visibility
does not grant permission to use, modify, or redistribute it: its restrictive
license remains authoritative. Foundation does not follow its `main` branch;
the submodule records one exact commit belonging to a tagged infrastructure
release.

The commands documented in this repository remain the public Foundation API.
Thin wrappers under `scripts/` supply Foundation's roots and cache options to
the pinned engine, so users still invoke `./scripts/build.sh`,
`./scripts/test.ps1`, and the other established commands directly. Packaging,
examples, documentation, firmware validation, and releases remain implemented
and governed by Foundation.

## Modules

- `Containers`: CircularBuffer and BitVector (external buffer or owned
  `cpstd::vector`).
- `Math`: ratios, complex values, matrices, and arithmetic helpers.
- `Functional`: callback utilities.
- `Time`: clocks, durations, frequencies, periods, ticks, and time points.
- `Scheduling`: Task, PeriodicTask, OneShotTask and TaskScheduler.
- `Utils`: program-memory (`FOUNDATION_FLASH`) helpers.

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

The native CTest suite covers Math, Utils, Containers, Functional,
Time, and Scheduling. GoogleTest is fetched only when testing is enabled, and
each module's tests are registered only when all required modules are enabled.
Examples remain separate API demonstrations.

Cross-compiled firmware has target-specific validators. For the exact
STM32G0B1CBT6 profile, this command exports the package, links a bare-metal
consumer, and inspects the final ELF without claiming hardware execution:

```bash
./scripts/test-stm32.sh \
  stm32g0b1cbt6_armgcc_cortex_m0plus_soft --fresh --parallel 4
```

## Export

Build and install one self-contained Release package under `dist/<preset>`:

```bash
./scripts/export.sh macos_arm64
./scripts/export.sh psoc5lp_armgcc_cortex_m3_soft
./scripts/export.sh atmega328p_avrgcc_avr5
```

The PSoC export is an experimental Cortex-M3/soft-float profile, not evidence
of compatibility with PSoC Creator or execution on PSoC 5LP hardware.

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

Include `<Foundation.h>` (or a root `Foundation_<Module>.h`) in the sketch
before any nested `<Foundation/...>` header so the Arduino builder finds the
library. The stock Arduino AVR core works as installed; no compiler overrides
are needed. `./scripts/test-arduino.sh` (or `.\scripts\test-arduino.ps1` on
Windows) compiles every example sketch for the Arduino Uno with the
unmodified core.

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
