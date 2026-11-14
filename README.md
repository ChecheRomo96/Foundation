# Foundation

**Documentation:** [API and build guide](https://checheromo96.github.io/Foundation/) · [API class index](https://checheromo96.github.io/Foundation/annotated.html)

Foundation is a lightweight C++ foundation library for embedded and desktop projects.
It provides reusable low-level building blocks shared by projects such as MIDILAR and RoboFoundation.

[Support matrix](https://checheromo96.github.io/Foundation/group__Foundation__BuildGuide__SupportMatrix.html)
· [Known limitations](https://checheromo96.github.io/Foundation/group__Foundation__BuildGuide__KnownLimitations.html)
· [Changelog](CHANGELOG.md)

Foundation 0.0.1 is the active RoModular baseline and builds on CPSTL 0.0.1.
Earlier tagged releases remain historical references; this baseline must pass
the build, test, export, checksum and standalone package-consumer workflow
before it is published.

Foundation requires a C++17-capable compiler for CMake packages and direct
source builds. Arduino source builds (`ARDUINO` defined) accept C++11 so stock
Arduino cores work unmodified. The language requirement does not imply a
dependency on the complete C++ standard library.

## CPSTL

Foundation 0.0.1 builds on [CPSTL](https://github.com/ChecheRomo96/CPSTL)
0.0.1 or newer for the standard vocabulary: `cpstd::vector`, `cpstd::move`,
`cpstd::swap`, `cpstd::stack`, `cpstd::queue` and the type traits. `cpstd`
aliases `std` in CPSTL's STL mode and is CPSTL's own implementation elsewhere,
such as AVR. Foundation does not reimplement standard types; it adds what the
standard library lacks.

CMake reuses a `CPSTL::CPSTL` target supplied by the parent project or fetches
its sources at tag `v0.0.1` (`-DFETCHCONTENT_SOURCE_DIR_CPSTL=../CPSTL` uses a
local working copy). Exported Foundation packages retain their separate CMake
package dependency for Release distribution. Arduino users install the CPSTL
library next to Foundation; `library.properties` declares it.

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
- `Utils`: program-memory (`ROMODULAR_FLASH`) helpers.

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

### Windows development workflow

Open **Developer PowerShell for Visual Studio**, then work from a recursive
checkout so the pinned build infrastructure is available:

```powershell
git clone --recurse-submodules https://github.com/ChecheRomo96/Foundation.git
Set-Location Foundation

./scripts/configure.ps1 windows_msvc_x64 -Fresh
./scripts/build.ps1 windows_msvc_x64 -Configuration Debug -Target Foundation
./scripts/test.ps1 windows_msvc_x64 -Configuration Debug
./scripts/build.ps1 windows_msvc_x64 -Configuration Debug `
    -ExamplesOn -Target FoundationExamples
```

`windows_msvc_x64` uses the Visual Studio multi-configuration generator, so
`-Configuration Debug` or `-Configuration Release` belongs on the build and
test commands; do not set `CMAKE_BUILD_TYPE`. Use `windows_clang_x64` instead
when developing with Clang on x64 Windows.

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

### Use Foundation during development

For normal application development on Windows, Linux, or macOS, consume
Foundation directly from source. CMake downloads the pinned source revision and
its CPSTL dependency; no export, installation, or `CMAKE_PREFIX_PATH`
configuration is required:

```cmake
include(FetchContent)

FetchContent_Declare(Foundation
  GIT_REPOSITORY https://github.com/ChecheRomo96/Foundation.git
  GIT_TAG v0.0.1
)
FetchContent_MakeAvailable(Foundation)

target_link_libraries(MyApplication PRIVATE Foundation::Foundation)
```

On Windows, configure and build the application normally with the desired
toolchain:

```powershell
cmake -S . -B build -G "Visual Studio 18 2026" -A x64
cmake --build build --config Debug
```

### Use an exported package for distribution

Use an exported Release package when distributing a controlled binary build.
It must use the same toolchain, architecture, and runtime as the application.
On Windows, export it first:

```powershell
./scripts/export.ps1 windows_msvc_x64 -Fresh
```

Then point the consumer at `dist/windows_msvc_x64` and link the exported target:

```cmake
cmake_minimum_required(VERSION 3.25)
project(MyApplication LANGUAGES CXX)

find_package(Foundation 0.0.1 CONFIG REQUIRED)

add_executable(MyApplication main.cpp)
target_link_libraries(MyApplication PRIVATE Foundation::Foundation)
target_compile_features(MyApplication PRIVATE cxx_std_17)
```

For example, configure the consumer from Developer PowerShell with:

```powershell
cmake -S . -B build -G "Visual Studio 18 2026" -A x64 `
    -DCMAKE_PREFIX_PATH="C:/path/to/Foundation/dist/windows_msvc_x64"
cmake --build build --config Release
```

The exported package is Release-only. Build Foundation from source when the
consumer requires Debug artifacts, and never mix MSVC and Clang packages.

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

Shared embedded build support is pinned to RoModularBuild `v0.0.2`.
The `atmega328p_avrgcc_avr5`, `atmega2560_avrgcc_avr6` and
`stm32g0b1cbt6_armgcc_cortex_m0plus_soft` presets retain repository-specific
feature and package policy. CI compile/package checks do not establish physical
board execution.
