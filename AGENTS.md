# Foundation agent instructions

Foundation is the low-level C++ library of the RoModular ecosystem. Keep it
independently buildable and usable by desktop and embedded consumers.

## Shared RoModular guidance

Before starting work, look for the shared guidance in
`../RoModularAgents`.

- If it exists and is readable, read `AGENTS.md` and `CONTRACT.md` completely.
- Read `repositories/Foundation.md` for the canonical repository adapter.
- Use the relevant skill under `skills/` when the request matches one.
- If the sibling repository is unavailable, continue with the rules in this
  file and report that the shared guidance was not loaded.

Shared guidance does not expand the user's requested scope. Do not modify MCC,
MIDILAR, RoModular, RoModularBuild, or another sibling repository unless the
user explicitly includes it.

## Repository rules

- Treat `CMakePresets.json`, its included preset files, and the scripts under
  `scripts/` as the supported build interface.
- Initialize the pinned `tools/RoModularBuild` submodule before invoking a
  workflow in a fresh checkout.
- Treat `tools/RoModularBuild` as a pinned, read-only dependency while working
  in Foundation. Shared engine or toolchain changes belong in the
  RoModularBuild repository; updating the submodule revision is a separate,
  explicit dependency change.
- Preserve C++17 for CMake packages and direct source builds. Arduino source
  builds intentionally support C++11 when `ARDUINO` is defined.
- Keep embedded paths free from mandatory exceptions and full-STL assumptions.
  Take the standard vocabulary (vector, move, swap, stack, queue, type traits)
  from CPSTL (`cpstd`); Foundation does not reimplement standard types.
  Do not disable heap use merely because a target is embedded.
- Preserve module selection through existing CMake cache options. Do not create
  configuration-specific package directory names.
- Release exports contain one Release Foundation library and CMake package
  metadata, plus the CPSTL package when Foundation built CPSTL from source.
  Debug is for development and testing and is not distributed.
- Examples demonstrate public APIs; they are not unit tests. Unit tests live
  under `tests/Foundation/` and use GoogleTest through CTest integration.
- Keep public headers, examples, tests, version metadata, `CHANGELOG.md`, and
  Doxygen documentation synchronized with public API changes.
- Treat hardware execution separately from compile/link validation for AVR,
  STM32, PSoC, and other embedded targets.
- Preserve unrelated work and do not commit, tag, push, publish, or merge unless
  the user explicitly requests it.

## Supported entry points

Use the PowerShell equivalent on Windows when one is provided.

```text
./scripts/configure.sh <preset> [--fresh] [-- <cmake-options>]
./scripts/build.sh <preset> [--fresh] [--config <configuration>]
./scripts/test.sh <preset> [--fresh] [--config <configuration>]
./scripts/install.sh <preset>
./scripts/export.sh <preset> [--fresh] [-- <cmake-options>]
./scripts/test-package.sh <preset> [--fresh]
./scripts/validate-package.sh <preset>
./scripts/validate-examples.sh <preset> [--fresh]
./scripts/test-arduino.sh [--fqbn <board>]
./scripts/test-stm32.sh <preset> [--fresh]
./scripts/docs.sh [--fresh]
```

Run the smallest relevant validation first, then broaden it according to risk.
State clearly which host, compiler, cross-compiler, and hardware checks were not
available in the current environment.
