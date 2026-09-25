# Changelog

This file records user-visible changes to Foundation. Release dates use the
`YYYY-MM-DD` format. Foundation has not published a stable release yet; the
scope below describes the planned first release.

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

## [1.0.0] - Unreleased

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

### Known limitations

- The final 1.0.0 release is blocked until the license and ownership model are
  reviewed by qualified legal counsel. The current repository remains
  proprietary and grants no external-use rights.
- Only the macOS Arm64 default configuration currently has recorded local
  Debug, Release, and isolated package-consumer evidence. The complete native
  CI and hardware validation matrix remains pending.
- Windows, Linux, macOS x86-64, AVR, STM32, Arduino, and Raspberry Pi support
  levels remain release candidates until their validation gates pass.
- PSoC and generic Arm profiles are experimental. PIC is deferred beyond
  version 1.0.0.
- Exported packages are static, target-specific, and Release-only. Foundation
  does not currently publish universal binaries, Debug packages, or a binary
  package manager integration.
- Sanitizer, warning-policy, coverage-threshold, public-header self-containment,
  and complete public-API coverage gates are not yet finished.

See the
[online Known Limitations page](https://checheromo96.github.io/Foundation/group__Foundation__BuildGuide__KnownLimitations.html)
and
[Support Matrix](https://checheromo96.github.io/Foundation/group__Foundation__BuildGuide__SupportMatrix.html)
for the maintained release criteria and platform details.

This is the first formally tracked release. Earlier repository history is
pre-release development and is not presented as a sequence of supported
versions.
