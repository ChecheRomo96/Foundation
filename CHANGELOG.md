# Changelog

This file records user-visible changes to Foundation. Release dates use the
`YYYY-MM-DD` format. Version 1.1.0 was the first published GitHub release;
version 1.2.0 is the current release.

## [Unreleased]

### Added

- Pull-request Doxygen validation with warning-as-error generation and a
  deployment job that runs only after successful validation on a branch push.
- Native Linux Arm64 GCC and Clang jobs using GitHub-hosted Arm64 runners.
- AVR and Arm bare-metal CI that builds Debug and Release, exports Release-only
  packages, and verifies object format, architecture, ABI metadata, and CMake
  import policy without claiming target execution.
- A public-API coverage map that separates verified evidence from the remaining
  `TEST-004` gap and records the closure of `TEST-002` and `TEST-003`.
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
- A license/ownership review packet with interim guardrails, counsel questions,
  evidence checklist, and objective exit criteria.

### Changed

- Release, support-matrix, validation, package, README, and backlog
  documentation now reflect the published Foundation 1.2.0 state.

### Known limitations

- External licensing remains blocked until the license and ownership model are
  reviewed by qualified legal counsel. Published source and packages remain
  proprietary and grant no external-use rights.
- macOS Arm64 is the only target with a recorded clean local native test and
  package-consumer run. The tagged workflow published packages for macOS
  Arm64, Windows x64, and Linux x64; remaining local, Arm64 Linux, examples,
  launcher, firmware, emulator, and hardware evidence stays explicitly open.
- PSoC and generic Arm profiles are experimental. PIC remains deferred.
- Exported packages are static, target-specific, and Release-only. Foundation
  does not currently publish universal binaries, Debug packages, or a binary
  package manager integration.
- Sanitizer, warning-policy, coverage-threshold, and public-header
  self-containment work remain open.

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
