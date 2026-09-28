#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/common.sh"

usage() {
    printf '%s\n' "Usage: $0 <preset> [--parallel <jobs>] [--fresh] [--skip-export] [--package <prefix>] [--toolchain-root <path>]"
}

PRESET=""
PARALLEL=""
FRESH=0
SKIP_EXPORT=0
PACKAGE_PREFIX=""
TOOLCHAIN_ROOT=""

while [ "$#" -gt 0 ]; do
    case "$1" in
        --parallel)
            foundation_require_value "$1" "${2:-}"
            PARALLEL=$2
            shift 2
            ;;
        --fresh)
            FRESH=1
            shift
            ;;
        --skip-export)
            SKIP_EXPORT=1
            shift
            ;;
        --package)
            foundation_require_value "$1" "${2:-}"
            PACKAGE_PREFIX=$2
            shift 2
            ;;
        --toolchain-root)
            foundation_require_value "$1" "${2:-}"
            TOOLCHAIN_ROOT=$(foundation_absolute_path "$2")
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        -*)
            foundation_die "unknown option: $1"
            ;;
        *)
            [ -z "$PRESET" ] || \
                foundation_die "only one preset may be specified"
            PRESET=$1
            shift
            ;;
    esac
done

foundation_require_preset "$PRESET"

case "$PRESET" in
    stm32g0b1cbt6_armgcc_cortex_m0plus_soft)
        CONSUMER_NAME="Stm32G0B1Consumer"
        CONSUMER_TARGET="FoundationStm32G0B1Consumer"
        ;;
    *)
        foundation_die "no firmware consumer is defined for preset '$PRESET'"
        ;;
esac

if [ "$SKIP_EXPORT" -eq 0 ] && [ -n "$PACKAGE_PREFIX" ]; then
    foundation_die "--package requires --skip-export"
fi

if [ "$SKIP_EXPORT" -eq 0 ]; then
    set -- "$SCRIPT_DIR/export.sh" "$PRESET"
    [ "$FRESH" -eq 0 ] || set -- "$@" --fresh
    [ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
    if [ -n "$TOOLCHAIN_ROOT" ]; then
        set -- "$@" -- \
            "-DFOUNDATION_ARM_TOOLCHAIN_ROOT=$TOOLCHAIN_ROOT"
    fi
    "$@"
fi

[ -n "$PACKAGE_PREFIX" ] || \
    PACKAGE_PREFIX="$FOUNDATION_DIST_ROOT/$PRESET"
PACKAGE_PREFIX=$(foundation_absolute_path "$PACKAGE_PREFIX")
[ -d "$PACKAGE_PREFIX" ] || \
    foundation_die "package prefix not found: $PACKAGE_PREFIX"

foundation_require_configured "$PRESET"
"$SCRIPT_DIR/validate-package.sh" "$PRESET" \
    --package "$PACKAGE_PREFIX"

FOUNDATION_BUILD_DIR=$(foundation_build_dir "$PRESET")
FOUNDATION_CACHE="$FOUNDATION_BUILD_DIR/CMakeCache.txt"
FOUNDATION_BUILD_INFO="$PACKAGE_PREFIX/lib/cmake/Foundation/FoundationBuildInfo.cmake"
CONSUMER_SOURCE_DIR="$FOUNDATION_ROOT/tests/$CONSUMER_NAME"
CONSUMER_BUILD_DIR="$FOUNDATION_BUILD_ROOT/stm32-consumer/$PRESET"

foundation_cache_value() {
    sed -n "s/^$1:[^=]*=//p" "$FOUNDATION_CACHE" | sed -n '1p'
}

foundation_package_info_value() {
    sed -n "s/^set($1 \"\(.*\)\")$/\1/p" \
        "$FOUNDATION_BUILD_INFO" | sed -n '1p'
}

GENERATOR=$(foundation_cache_value CMAKE_GENERATOR)
TOOLCHAIN_FILE=$(foundation_cache_value CMAKE_TOOLCHAIN_FILE)
CXX_COMPILER=$(foundation_cache_value CMAKE_CXX_COMPILER)
CACHE_TOOLCHAIN_ROOT=$(foundation_cache_value FOUNDATION_ARM_TOOLCHAIN_ROOT)
SYSTEM_NAME=$(foundation_package_info_value Foundation_SYSTEM_NAME)

[ "$SYSTEM_NAME" = "Generic" ] || \
    foundation_die "STM32 validation requires a cross-compiled preset"
[ -f "$TOOLCHAIN_FILE" ] || \
    foundation_die "toolchain file is unavailable: $TOOLCHAIN_FILE"

if [ -z "$TOOLCHAIN_ROOT" ]; then
    TOOLCHAIN_ROOT=$CACHE_TOOLCHAIN_ROOT
fi

cmake -E remove_directory "$CONSUMER_BUILD_DIR"

set -- cmake \
    -S "$CONSUMER_SOURCE_DIR" \
    -B "$CONSUMER_BUILD_DIR" \
    -G "$GENERATOR" \
    "-DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE" \
    "-DFOUNDATION_PACKAGE_PREFIX=$PACKAGE_PREFIX"

[ -z "$TOOLCHAIN_ROOT" ] || set -- "$@" \
    "-DFOUNDATION_ARM_TOOLCHAIN_ROOT=$TOOLCHAIN_ROOT"

"$@"

set -- cmake --build "$CONSUMER_BUILD_DIR" \
    --config Release --target "$CONSUMER_TARGET"
[ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
"$@"

ELF=$(find "$CONSUMER_BUILD_DIR" -type f \
    -name "$CONSUMER_TARGET.elf" -print | sed -n '1p')
[ -n "$ELF" ] || foundation_die "consumer ELF was not generated"
ARTIFACT_DIR=$(dirname -- "$ELF")
MAP="$ARTIFACT_DIR/$CONSUMER_TARGET.map"
HEX="$ARTIFACT_DIR/$CONSUMER_TARGET.hex"
BIN="$ARTIFACT_DIR/$CONSUMER_TARGET.bin"

for artifact in "$MAP" "$HEX" "$BIN"; do
    [ -f "$artifact" ] || \
        foundation_die "consumer artifact was not generated: $artifact"
done

COMPILER_DIRECTORY=$(dirname -- "$CXX_COMPILER")
OBJDUMP="$COMPILER_DIRECTORY/arm-none-eabi-objdump"
READELF="$COMPILER_DIRECTORY/arm-none-eabi-readelf"
NM="$COMPILER_DIRECTORY/arm-none-eabi-nm"
SIZE="$COMPILER_DIRECTORY/arm-none-eabi-size"

for tool in "$OBJDUMP" "$READELF" "$NM" "$SIZE"; do
    [ -x "$tool" ] || foundation_die "required Arm tool is unavailable: $tool"
done

HEADER=$($READELF -h "$ELF")
ATTRIBUTES=$($READELF -A "$ELF")
SECTIONS=$($OBJDUMP -h "$ELF")
SYMBOLS=$($NM -g "$ELF")
UNDEFINED=$($NM -u "$ELF")

printf '%s\n' "$HEADER"
printf '%s\n' "$ATTRIBUTES"
printf '%s\n' "$SECTIONS"
$SIZE "$ELF"

printf '%s\n' "$HEADER" | grep -F 'Class:                             ELF32' >/dev/null || \
    foundation_die "consumer is not ELF32"
printf '%s\n' "$HEADER" | grep -F 'Machine:                           ARM' >/dev/null || \
    foundation_die "consumer is not an Arm executable"
printf '%s\n' "$HEADER" | grep -F 'soft-float ABI' >/dev/null || \
    foundation_die "consumer does not use the soft-float ABI"
printf '%s\n' "$ATTRIBUTES" | grep -F 'Tag_CPU_arch: v6S-M' >/dev/null || \
    foundation_die "consumer is not compiled for Cortex-M0+ / Armv6-M"
if printf '%s\n' "$ATTRIBUTES" | \
   grep -F 'Tag_ABI_VFP_args: VFP registers' >/dev/null; then
    foundation_die "consumer unexpectedly advertises the hard-float ABI"
fi
printf '%s\n' "$SECTIONS" | grep -E \
    '^  [0-9]+ \.isr_vector +00000040 +08000000 ' >/dev/null || \
    foundation_die "vector table is not a 64-byte table at 0x08000000"
printf '%s\n' "$SECTIONS" | grep -E \
    '^  [0-9]+ \.data +[0-9a-f]+ +20000000 ' >/dev/null || \
    foundation_die "initialized data does not begin in SRAM at 0x20000000"
[ -z "$UNDEFINED" ] || \
    foundation_die "consumer contains undefined symbols:$UNDEFINED"

for symbol in \
    Reset_Handler \
    FoundationValidationHalt \
    FoundationValidationState \
    FoundationValidationPassed \
    FoundationValidationTotal
do
    printf '%s\n' "$SYMBOLS" | grep -E " [A-Za-z] $symbol$" >/dev/null || \
        foundation_die "consumer symbol is missing: $symbol"
done

grep -F "$PACKAGE_PREFIX/lib/libFoundation.a" "$MAP" >/dev/null || \
    foundation_die "consumer map does not reference the exported Foundation archive"
grep -F 'Frequency.cpp' "$MAP" >/dev/null || \
    foundation_die "consumer did not link Foundation Frequency symbols"
grep -F 'Period.cpp' "$MAP" >/dev/null || \
    foundation_die "consumer did not link Foundation Period symbols"

printf '%s\n' "Validated STM32 firmware linkage for $PRESET"
printf '%s\n' "Firmware artifacts: $ARTIFACT_DIR"
printf '%s\n' "Target execution remains required; this script does not claim hardware evidence."
