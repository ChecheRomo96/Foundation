# Changelog

This file records user-visible changes to Foundation. Release dates use the
`YYYY-MM-DD` format. Version 1.1.0 was the first published GitHub release.

## [2.0.4] - 2026-10-07

### Fixed

- Updated the CPSTL dependency to 1.1.4, which scopes its freestanding
  allocator declarations so Windows hosted C runtimes retain their ABI
  attributes.

## [2.0.3] - 2026-10-07

### Fixed

- The STM32G0B1 validation firmware now supplies a bounded `malloc` heap via
  `_sbrk`, making the freestanding CPSTL C allocator linkable without a C++
  runtime. This is isolated to the firmware consumer; production boards retain
  control of their own heap policy.

## [2.0.2] - 2026-10-07

### Fixed

- Updated the CPSTL dependency to 1.1.3. That release declares the C allocator
  hooks on freestanding Arm targets and prevents GCC from reporting a false
  placement-new overflow while compiling the external-storage container tests.

## [2.0.1] - 2026-10-07

### Fixed

- Foundation now requires CPSTL 1.1.2. The dependency defaults to C allocation
  (`malloc`/`free`) for every freestanding Arm and AVR source build, so the
  STM32G0B1 firmware consumer links without `operator new` or `operator
  delete` from a C++ runtime.
- Embedded CI and the Arduino metadata resolve CPSTL 1.1.2 rather than 1.1.0.

## [2.0.0] - 2026-10-07

Foundation now builds on CPSTL and stops reimplementing standard types.

### Changed

- Foundation depends on CPSTL 1.1.0 or newer (1.x) for the standard
  vocabulary (`cpstd::vector`, `cpstd::move`, type traits). `cpstd` aliases
  `std` in CPSTL's STL mode and is CPSTL's own implementation elsewhere, such
  as AVR. CMake resolves CPSTL from a parent project, `FOUNDATION_CPSTL_PREFIX`,
  a sibling export at `../CPSTL/dist/<preset>`, the normal package search, or
  its sources at tag `v1.1.0` (`FOUNDATION_FETCH_CPSTL`, default `ON`;
  `FETCHCONTENT_SOURCE_DIR_CPSTL` selects a local working copy). A Foundation
  package that built CPSTL installs it alongside, and `FoundationConfig.cmake`
  finds it. On AVR the bundled CPSTL defaults to C (`malloc`) allocation.
- `library.properties` declares `depends=CPSTL (>=1.1.0)`, and every root
  header in `src/` includes `Foundation_BuildSettings.h`, which includes
  `CPSTL_BuildSettings.h`, so the Arduino builder discovers CPSTL from any
  Foundation include. `scripts/test-arduino.sh` and `.ps1` take `--cpstl`
  (default `FOUNDATION_CPSTL_SOURCE` or `../CPSTL`).
- `Foundation::Containers::BitVector` (unreleased in 1.x) stores its bits in
  an owned `cpstd::vector<uint8_t>` or, when given a non-null pointer through
  the constructor or `Attach()`, in that external buffer, which is never grown
  or released. `Attach(nullptr, ...)` returns to owned storage; `IsExternal()`
  reports the mode. Owned growth follows `cpstd::vector`.
- Containers no longer requires Utils, and Time no longer requires TypeTraits.
- The `Foundation_Containers_DataStructures` example and sketch show
  CircularBuffer and BitVector over an external buffer and over owned storage.

### Removed

- The TypeTraits module (`Foundation/TypeTraits.h`, `Foundation_TypeTraits.h`,
  `FOUNDATION_TYPE_TRAITS`, `FOUNDATION_HAS_CPP17_VARIABLE_TRAITS`) and its
  `Foundation_TypeTraits_BasicChecks` example: use the CPSTL type traits
  (`<CPtype_traits.h>`).
- `Foundation::Utils::Move` and `Foundation::Utils::Swap` and the
  `Foundation_Utils_MoveAndSwap` example: use `cpstd::move` and `cpstd::swap`.
- `Foundation::Containers::Stack` and `Foundation::Containers::Queue`: use
  `cpstd::stack` and `cpstd::queue`, or `CircularBuffer` for a bounded FIFO
  over caller storage.
- The unreleased `Foundation::Containers::Vector<T>`: use `cpstd::vector`.
- `BasicRatio<T>::ToFloat()`, deprecated since 1.3.0: use `Value<float>()`.

### Added

- `scripts/test-arduino.ps1`, the PowerShell equivalent of
  `scripts/test-arduino.sh`, and a Windows leg of the Arduino CI job that runs
  it.

## [1.4.0] - 2026-10-01

### Added

- `Foundation/Utils/Flash.h`: the `FOUNDATION_FLASH` placement macro
  (`PROGMEM` on AVR, empty elsewhere) and `Foundation::Utils::Flash::Read`,
  `Copy`, `StringLength`, and `CopyString`, so constant tables and strings stay
  in program memory on AVR and are read identically on every target.
- `Foundation_Utils_FlashData` desktop example and Arduino sketch.
- Flash checks in the `tests/AvrConsumer` firmware.

## [1.3.0] - 2026-09-30

### Changed

- Foundation and its examples now use `BasicRatio<T>::Value<TResult>()` for
  scalar conversion. `ToFloat()` remains as a Foundation 1.x compatibility
  bridge and is planned for removal in 2.0.
- Arduino source builds (`ARDUINO` defined) now accept C++11, so the stock
  Arduino IDE and AVR core compile Foundation without editing the core. Other
  integration modes still require C++17. C++14 relaxed-`constexpr` mutators use
  the new `FOUNDATION_CONSTEXPR14` macro, nested namespace definitions were
  split, and the `_v` TypeTraits variables are declared only from C++17.
- Every example sketch includes `<Foundation.h>` first so the Arduino builder
  discovers the library.
- PSoC 5LP is explicitly retained as experimental. Its named preset currently
  proves a generic Cortex-M3/soft-float package only; promotion requires an
  exact C++17-capable PSoC Creator toolchain, consumer link, and hardware run.

### Fixed

- The Time Period and Frequency sketches passed a signed `Ratio` printer where
  an `UnsignedRatio` printer was required; the stock core's `-fpermissive`
  had accepted the mismatch.

### Added

- An exact `stm32g0b1cbt6_armgcc_cortex_m0plus_soft` preset and a
  repository-owned bare-metal firmware consumer. The consumer links the
  exported package, emits ELF/HEX/BIN/map artifacts, and exposes its runtime
  result through SWD-readable symbols without requiring HAL, CMSIS, UART, or a
  board-specific LED.
- `BasicRatio<T>::Value<TResult>()`, which preserves the selected numerator and
  denominator representation while evaluating fractional results in the
  requested type. Its default result type is `float`; integral results use a
  safe 64-bit intermediate and normal truncating division.
- `scripts/test-arduino.sh` and an `arduino` CI job that compile every example
  sketch for the Arduino Uno with the unmodified AVR core.
- A manual-only CI acceptance workflow that creates controlled Bash and
  PowerShell failures, retains their CMake, CTest, JUnit, and partial-package
  diagnostics, and verifies both uploaded artifacts without affecting normal
  push or pull-request runs.
- Cross-platform example validators that discover every desktop example, build
  and execute Debug and Release, compare their output, validate the Release-only
  export, run the exported executables, and exercise native menu launchers
  without registering examples as tests.
- Pull-request Doxygen validation with warning-as-error generation and a
  deployment job that runs only after successful validation on a branch push.
- Native Linux Arm64 GCC and Clang jobs using GitHub-hosted Arm64 runners.
- AVR and Arm bare-metal CI that builds Debug and Release, exports Release-only
  packages, and verifies object format, architecture, ABI metadata, and CMake
  import policy without claiming target execution.
- A public-API coverage map that records the closure of `TEST-002`, `TEST-003`,
  and `TEST-004` with direct behavior, conformance, and header evidence.
- Twenty-three focused Math behavior cases for Complex, trigonometry, Ratio
  mutators and numeric limits, and fixed/dynamic matrix member APIs.
- Sixteen focused behavior cases for explicit container-copy semantics,
  Callback null and empty modes, Time mutators and boundaries, scheduler
  ordering/capacity, and move-only or throwing utility types. Callback exception
  behavior is compiled separately from the default no-exception contract.
- A `FoundationHeaders.SelfContained` test that compiles every enabled public
  `.h` header alone in a generated translation unit, closing `TEST-004`.
- A warnings-as-errors policy for Foundation and its tests when testing is
  enabled, and a CI job running the suite under AddressSanitizer and
  UndefinedBehaviorSanitizer on Linux GCC/Clang and macOS.
- A GCC coverage job that publishes an HTML gcovr report and fails below
  90% line coverage of the library sources.
- A feature-matrix check that builds and tests default, full, core-only, and
  every module-disabled selection, and verifies the configure-time error for
  unsupported selections.
- Optional JUnit output in native and package-consumer test scripts, plus
  failure-only CI artifacts containing available CTest, CMake, package,
  inspection, coverage, and documentation diagnostics.
- Strong fresh-state semantics that remove the complete selected build tree,
  with native CI regression checks against stale build and package artifacts.
- Cross-platform package-identity validation that requires one Release archive,
  an exact configured header and metadata set, no Debug imports or binaries,
  matching compiler/ABI metadata, and the expected Mach-O, ELF, AVR, Arm, or
  COFF architecture before a package consumer or release archive can pass.
- A license/ownership review packet with interim guardrails, counsel questions,
  evidence checklist, and objective exit criteria.

### Changed

- Release, support-matrix, validation, package, README, and backlog
  documentation now reflect the published Foundation 1.3.0 state.
- Version metadata is now `1.3.0`. The CMake package keeps
  `SameMajorVersion` compatibility, so 1.x consumers such as MCC continue to
  accept it.

### Known limitations

- External licensing remains blocked until the license and ownership model are
  reviewed by qualified legal counsel. Published source and packages remain
  proprietary and grant no external-use rights.
- macOS Arm64 and macOS x64 through Rosetta 2 have recorded clean native,
  package-consumer, example, and launcher validation. Hosted Windows and Linux
  builds pass their tests, package consumers, and examples; only the manual
  Windows Explorer launcher acceptance remains open.
- PSoC and generic Arm profiles are experimental. PIC remains deferred.
- Exported packages are static, target-specific, and Release-only. Foundation
  does not currently publish universal binaries, Debug packages, or a binary
  package manager integration.
- All Arduino Uno sketches compile with the stock AVR core, and the exported
  AVR package has executed a 14-check firmware consumer on an Arduino Uno.
  Raspberry Pi-specific evidence, STM32/PSoC firmware execution, and automated
  AVR emulator execution remain open; cross-compilation alone does not replace
  those gates.

See the
[online Known Limitations page](https://checheromo96.github.io/Foundation/group__Foundation__BuildGuide__KnownLimitations.html)
and
[Support Matrix](https://checheromo96.github.io/Foundation/group__Foundation__BuildGuide__SupportMatrix.html)
for the maintained release criteria and platform details.

## [1.2.0] - 2026-09-25

### Added

- `Foundation::Math::FloorDiv(int32_t value, int32_t divisor)` in
  `Foundation/Math/Arithmetic.h`: a `constexpr`, `noexcept` floored division
  that rounds toward negative infinity for every `int32_t` dividend and
  positive divisor. It pairs with `FloorMod` through
  `FloorDiv(v, d) * d + FloorMod(v, d) == v`. A non-positive divisor returns
  `0` without evaluating division by zero or `INT32_MIN / -1`.

### Changed

- Version metadata is now `1.2.0`. The CMake package keeps
  `SameMajorVersion` compatibility, so consumers requesting
  `find_package(Foundation 1.0)` accept this release.

## [1.1.0] - 2026-09-25

### Added

- `Foundation::Math::FloorMod(int32_t value, int32_t modulus)` in
  `Foundation/Math/Arithmetic.h`: a `constexpr`, `noexcept` floored modulo
  that returns a result in `[0, modulus - 1]` for every `int32_t` value,
  including negative values, `INT32_MIN`, and `INT32_MAX`. A non-positive
  modulus returns `0` instead of invoking undefined behavior.

### Changed

- Version metadata is now `1.1.0`. The CMake package keeps
  `SameMajorVersion` compatibility, so consumers requesting
  `find_package(Foundation 1.0)` accept this release.

## [1.0.0] - Superseded before publication

The planned `v1.0.0` tag was never published. Its completed engineering scope
became the baseline for the first published release, `v1.1.0`.

### Added

- Modular C++17 APIs for TypeTraits, Math, Containers, Functional, Time,
  Scheduling, and Utils.
- Fixed and dynamic matrices, including caller-owned storage and non-throwing
  allocation-failure reporting.
- Allocation-free callbacks, fixed-capacity containers, modular tick types,
  clocks, durations, time points, and task scheduling.
- CMake presets for native macOS, Windows, and Linux builds plus named AVR,
  STM32, PSoC, and generic Arm cross-compilation profiles.
- Matching Bash and PowerShell workflows for configuration, compilation,
  testing, installation, export, documentation, and package validation.
- Modular GoogleTest coverage with CTest discovery and explicit checks for
  invalid CMake feature combinations.
- Cross-platform API examples with optional Release export and interactive
  desktop launchers.
- Release-only CMake packages exposing `Foundation::Foundation` through
  `find_package(Foundation CONFIG REQUIRED)`.
- A standalone package-consumer fixture that validates umbrella and selective
  headers while forcing linkage of non-inline Foundation symbols.
- Doxygen API, build, workflow, validation, support-matrix, and release-planning
  documentation published through GitHub Pages.

### Changed

- Standardized the public API, module umbrellas, naming, ownership contracts,
  invalid states, and exception guarantees in preparation for the 1.0 freeze.
- Standardized every integration mode on C++17 and version `1.0.0` metadata.
- Defined signed `Ratio` and unsigned `UnsignedRatio` specializations.
- Generalized Time and Scheduling around configurable unsigned tick widths,
  with `Tick32` as the default.
- Kept Debug artifacts in the development build tree while limiting exported
  packages to the Release library and matching metadata.
- Established CMake presets and toolchain files as the source of truth for
  target architecture, compiler, ABI, and feature defaults.
